///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "GUISystem.hpp"
#include "GUI.hpp"

using namespace ftxui;

// Take a list of component, display them vertically, one column shifted to the
// right.
Component RepresentInner(std::vector<Component> children) {
   Component vlist = Container::Vertical(std::move(children));
   return Renderer(vlist, [vlist] {
      return hbox({
         text(" "),
         vlist->Render(),
      });
   });
}

/// Represent a Trait as GUI                                                  
///   @param trait - the trait to represent                                   
///   @return the FTXUI representation                                        
Component Represent(const Trait&) {
   //TODO
   return std::make_shared<ComponentBase>();
}

/// Represent a Unit as GUI                                                   
///   @param unit - the unit to represent                                     
///   @return the FTXUI representation                                        
Component Represent(const Unit&) {
   //TODO
   return std::make_shared<ComponentBase>();
}

/// Represent a Thing as GUI                                                  
///   @param thing - the thing to represent                                   
///   @return the FTXUI representation                                        
Component Represent(const Thing& thing) {
   std::vector<Component> properties;

   // Represent traits                                                  
   std::vector<Component> traits;
   for (auto traitlist : thing.GetTraits()) {
      for(auto& trait : traitlist.mValue)
         traits.push_back(Represent(trait));
   }
   if (not traits.empty())
      properties.push_back(Collapsible("Traits", RepresentInner(traits)));

   // Represent units                                                   
   std::vector<Component> units;
   for (auto& unit : thing.GetUnits())
      units.push_back(Represent(*unit));
   if (not units.empty())
      properties.push_back(Collapsible("Units", RepresentInner(units)));

   // Represent child-things                                            
   std::vector<Component> children;
   for (auto& child : thing.GetChildren())
      children.push_back(Represent(*child));
   if (not children.empty())
      properties.push_back(Collapsible("Children", RepresentInner(children)));

   // Combine all thing's properties                                    
   auto objectGroup = Collapsible("Thing",
      RepresentInner(properties)
   );

   return Renderer(objectGroup, [objectGroup] {
      return hbox({
          text(" "),
          objectGroup->Render(),
      });
   });
}

/// GUI system construction                                                   
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the GUI                
GUISystem::GUISystem(GUI* producer, const Neat& descriptor)
   : A::UI::System {MetaOf<GUISystem>(), descriptor}
   , ProducedFrom {producer, descriptor}
   , mItems {this}
   , mScreen {ScreenInteractive::Fullscreen()} {   
   // Create the tab selector                                           
   mTabNames = {"Log", "Flow"};
   mTabSelector = Toggle(&mTabNames, &mSelectedTab);

   // The log tab                                                       
   mLogTab = Container::Vertical({});
   auto logTabRenderer = Renderer(mLogTab, [&] {
      return vbox(mLog) | vscroll_indicator | frame | flex;
   });

   // The flow tab                                                      
   mFlowContents = Container::Vertical({});
   mFlowCommand = Input(&mFlowCommandInput, " -input here- ");
   mFlowTab = Container::Vertical({
      mFlowContents,
      mFlowCommand
   });
   auto flowTabRenderer = Renderer(mFlowTab, [&] {
      return vbox({
         mFlowContents->Render(),
         filler(),
         separatorCharacter(" ") | color(Color::DarkOrange) | underlined,
         hbox(
            text(">") | color(Color::DarkOrange) | bold,
            mFlowCommand->Render()
         ),
         separatorCharacter(" ")
      });
   });

   // The combined tabs                                                 
   mTabContents = Container::Tab({
      logTabRenderer,
      flowTabRenderer
   }, &mSelectedTab);

   // The left panel, composed of mainly tabs                           
   mLeftPanel = Container::Vertical({
      mTabSelector,
      mTabContents
   });
   auto leftRenderer = Renderer(mLeftPanel, [this] {
      return vbox({
         mTabSelector->Render() | border,
         mTabContents->Render()
      });
   });

   // The right panel, composed of the hierarchy tree, and selection    
   mTree = Container::Vertical({});
   for (auto& thing : GetOwners()) {
      mTree->Add(Represent(*thing));
   }
   mSelection = Container::Vertical({});
   mRightPanel = Container::Vertical({
      mTree,
      mSelection
   });
   auto rightRenderer = Renderer(mRightPanel, [this] {
      return vbox({
         text("Hierarchy:"),
         separator(),
         mTree->Render(),
         filler(),
         separator(),
         mSelection->Render()
      });
   });

   // Combine both panels in a resizable split                          
   auto main = ResizableSplitRight(rightRenderer, leftRenderer, &mSplit);
   
   // Create the main loop                                              
   try {
      mLoop = new ftxui::Loop(&mScreen, std::move(main));
   }
   catch (const std::exception& e) {
      Logger::Error(Self(), "Unable to create FTXUI main loop: ", e.what());
      throw;
   }

   Couple(descriptor);
}

/// Shutdown the module                                                       
GUISystem::~GUISystem() {
   if (mLoop)
      delete mLoop;
}

/// Produce GUI elements in the system                                        
///   @param verb - creation verb to satisfy                                  
void GUISystem::Create(Verb& verb) {
   mItems.Create(verb);
}

/// System update routine                                                     
///   @param deltaTime - time between updates                                 
///   @return false if the system has been terminated by user request         
bool GUISystem::Update(Time deltaTime) {
   if (mLoop and mLoop->HasQuitted())
      return false;

   // Update all UI elements                                            
   for (auto& item : mItems)
      item.Update(deltaTime);

   // Yield FTXUI                                                       
   mLoop->RunOnce();
   return true;
}

/// React on environmental change                                             
void GUISystem::Refresh() {

}