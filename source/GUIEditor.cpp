///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "GUIEditor.hpp"
#include "GUI.hpp"

#include <Langulus/Annies/Functional/Deepen.hpp>


// Take a list of component, display them vertically, one column shifted to the
// right.
ftxui::Component RepresentInner(std::vector<ftxui::Component> children) {
   ftxui::Component vlist = ftxui::Container::Vertical(std::move(children));
   return ftxui::Renderer(vlist, [vlist] {
      return ftxui::hbox({
         ftxui::text(" "),
         vlist->Render(),
      });
   });
}

/// Represent a Tag as GUI                                                    
///   @param tag the tag to represent                                         
///   @return the FTXUI representation                                        
ftxui::Component Represent(Annies::Tag const&) {
   //TODO
   return std::make_shared<ftxui::ComponentBase>();
}

/// Represent a Part as GUI                                                   
///   @param part the part to represent                                       
///   @return the FTXUI representation                                        
ftxui::Component Represent(Things::Part const&) {
   //TODO
   return std::make_shared<ftxui::ComponentBase>();
}

/// Represent a Thing as GUI                                                  
///   @param thing the thing to represent                                     
///   @return the FTXUI representation                                        
ftxui::Component Represent(Things::Thing const& thing) {
   std::vector<ftxui::Component> properties;

   // Represent tags                                                    
   std::vector<ftxui::Component> tags;
   for (auto taglist : thing.GetTags()) {
      for(auto& tag : taglist.GetVal())
         tags.push_back(Represent(tag));
   }
   if (not tags.empty())
      properties.push_back(Collapsible("Tags", RepresentInner(tags)));

   // Represent parts                                                   
   std::vector<ftxui::Component> parts;
   for (auto part : thing.GetParts())
      parts.push_back(Represent(*part));
   if (not parts.empty())
      properties.push_back(Collapsible("Parts", RepresentInner(parts)));

   // Represent child-things                                            
   std::vector<ftxui::Component> children;
   for (auto child : thing.GetChildren())
      children.push_back(Represent(*child));
   if (not children.empty())
      properties.push_back(Collapsible("Children", RepresentInner(children)));

   // Combine all thing's properties                                    
   auto objectGroup = Collapsible("Thing", RepresentInner(properties));

   return ftxui::Renderer(objectGroup, [objectGroup] {
      return ftxui::hbox({
          ftxui::text(" "),
          objectGroup->Render(),
      });
   });
}


/// GUI item construction                                                     
///   @param producer the system producer                                     
///   @param descriptor instructions for configuring the item                 
GUIEditor::GUIEditor(GUISystem const* producer, Neat const& descriptor)
   : Downcastable {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_GUI("Initializing...");
   
   // Create the tab selector                                           
   mTabNames = {"Log", "Flow"};
   mTabSelector = ftxui::Toggle(&mTabNames, &mSelectedTab);

   // The log tab                                                       
   mLogTab = ftxui::Container::Vertical({});
   auto logTabRenderer = ftxui::Renderer(mLogTab, [&] {
      return ftxui::vbox(mLog) | ftxui::vscroll_indicator | ftxui::frame | ftxui::flex;
   });

   // The flow tab                                                      
   mFlowContents = ftxui::Container::Vertical({});
   mFlowCommand = ftxui::Input(&mFlowCommandInput, " -input here- ");
   mFlowTab = ftxui::Container::Vertical({
      mFlowContents,
      mFlowCommand
   });

   auto flowTabRenderer = ftxui::Renderer(mFlowTab, [&] {
      return ftxui::vbox({
         mFlowContents->Render(),
         ftxui::filler(),
         ftxui::separatorCharacter(" ") | ftxui::color(ftxui::Color::DarkOrange) | ftxui::underlined,
         ftxui::hbox(
            ftxui::text(">") | ftxui::color(ftxui::Color::DarkOrange) | ftxui::bold,
            mFlowCommand->Render()
         ),
         ftxui::separatorCharacter(" ")
      });
   });

   // The combined tabs                                                 
   mTabContents = ftxui::Container::Tab({
      logTabRenderer,
      flowTabRenderer
   }, &mSelectedTab);

   // The left panel, composed of mainly tabs                           
   mLeftPanel = ftxui::Container::Vertical({
      mTabSelector,
      mTabContents
   });

   auto leftRenderer = ftxui::Renderer(mLeftPanel, [this] {
      return ftxui::vbox({
         mTabSelector->Render() | ftxui::border,
         mTabContents->Render()
      });
   });

   // The right panel, composed of the hierarchy tree, and selection    
   mTree = ftxui::Container::Vertical({});
   for (auto thing : GetOwners())
      mTree->Add(Represent(*thing));

   mSelection = ftxui::Container::Vertical({});
   mRightPanel = ftxui::Container::Vertical({
      mTree,
      mSelection
   });

   auto rightRenderer = ftxui::Renderer(mRightPanel, [this] {
      return ftxui::vbox({
         ftxui::text("Hierarchy:"),
         ftxui::separator(),
         mTree->Render(),
         ftxui::filler(),
         ftxui::separator(),
         mSelection->Render()
      });
   });

   // Combine both panels in a resizable split                          
   mMain = ftxui::ResizableSplitRight(rightRenderer, leftRenderer, &mSplit);

   Couple(descriptor);
   VERBOSE_GUI("Initialized");
}

/// React on environmental change                                             
void GUIEditor::Refresh() {

}

