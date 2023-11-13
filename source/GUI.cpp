///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "GUI.hpp"

LANGULUS_DEFINE_MODULE(
   GUI, 9, "FTXUI",
   "GUI generator and simulator, using FTXUI as backend", "",
   GUI, GUISystem, GUIItem
)

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
      units.push_back(Represent(**unit));
   if (not units.empty())
      properties.push_back(Collapsible("Units", RepresentInner(units)));

   // Represent child-things                                            
   std::vector<Component> children;
   for (auto& child : thing.GetChildren())
      children.push_back(Represent(**child));
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


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
GUI::GUI(Runtime* runtime, const Neat&)
   : A::UI::Module {MetaOf<GUI>(), runtime}
   , mSystems {this}
   , mScreen {ScreenInteractive::Fullscreen()} {
   Logger::Verbose(Self(), "Initializing...");

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
   mTree = Represent(*runtime->GetOwner());
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

   // Create the main loop                                              
   auto split = ResizableSplitRight(rightRenderer, leftRenderer, &mSplit);
   try { mLoop = new Loop(&mScreen, std::move(split)); }
   catch (const std::exception& e) {
      Logger::Error(Self(), "Unable to create FTXUI main loop: ", e.what());
      throw;
   }

   Logger::Verbose(Self(), "Initialized");
}

/// Shutdown the module                                                       
GUI::~GUI() {
   if (mLoop)
      delete mLoop;
}

/// Module update routine                                                     
///   @param dt - time from last update                                       
///   @return false if the UI requested exit                                  
bool GUI::Update(Time) {
   if (mSplit != mSplitPrev) {
      // Need to refresh paragraphs                                     
      std::string p = R"(In probability theory and statistics, Bayes' theorem (alternatively Bayes' law or Bayes' rule) describes the probability of an event, based on prior knowledge of conditions that might be related to the event. For example, if cancer is related to age, then, using Bayes' theorem, a person's age can be used to more accurately assess the probability that they have cancer, compared to the assessment of the probability of cancer made without knowledge of the person's age. One of the many applications of Bayes' theorem is Bayesian inference, a particular approach to statistical inference. When applied, the probabilities involved in Bayes' theorem may have different probability interpretations. With the Bayesian probability interpretation the theorem expresses how a subjective degree of belief should rationally change to account for availability of related evidence. Bayesian inference is fundamental to Bayesian statistics.)";
      mSplitPrev = mSplit;
      mLog.clear();
      mLog.push_back(paragraph(p) | size(WIDTH, EQUAL, mScreen.dimx() - mSplit - 3));
   }

   if (mLoop and mLoop->HasQuitted())
      return false;

   // Yield FTXUI                                                       
   mLoop->RunOnce();
   return true;
}

/// Create/Destroy GUI systems                                                
///   @param verb - the creation/destruction verb                             
void GUI::Create(Verb& verb) {
   mSystems.Create(verb);
}