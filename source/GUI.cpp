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
Component Represent(const Trait& trait) {
   //TODO
   return std::make_shared<ComponentBase>();
}

/// Represent a Unit as GUI                                                   
///   @param unit - the unit to represent                                     
///   @return the FTXUI representation                                        
Component Represent(const Unit& unit) {
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

   // Create the component (and input) hierarchy                        
   mCommand = Input(&mInput, " -input here- ");
   mTabNames = {"Log", "Test"};
   mTabs = Dropdown(&mTabNames, &mSelectedTab);
   mTree = Represent(*runtime->GetOwner());

   auto layout = Container::Vertical(
      {mCommand, mTabs, mTree}
   );

   std::string p = R"(In probability theory and statistics, Bayes' theorem (alternatively Bayes' law or Bayes' rule) describes the probability of an event, based on prior knowledge of conditions that might be related to the event. For example, if cancer is related to age, then, using Bayes' theorem, a person's age can be used to more accurately assess the probability that they have cancer, compared to the assessment of the probability of cancer made without knowledge of the person's age. One of the many applications of Bayes' theorem is Bayesian inference, a particular approach to statistical inference. When applied, the probabilities involved in Bayes' theorem may have different probability interpretations. With the Bayesian probability interpretation the theorem expresses how a subjective degree of belief should rationally change to account for availability of related evidence. Bayesian inference is fundamental to Bayesian statistics.)";

   // Create the renderers                                              
   auto left = Renderer(layout, [&] {
      return vbox({
         mTabs->Render(),
         vbox(
            //flexbox({text(mLog), paragraph(p)}) | notflex,
            text(mLog),
            filler()
         ) | flex,
         separatorCharacter(" ") | color(Color::DarkOrange) | underlined,
         hbox(
            text(">") | color(Color::DarkOrange) | bold,
            mCommand->Render()
         )
      });
   });

   auto right = Renderer(layout, [&] {
      return vbox({
         text("Hierarchy:"),
         separator(),
         mTree->Render()
      });
   });

   // Create the main loop                                              
   auto split = ResizableSplitRight(right, left, &mSplit);
   mLoop = new Loop(&mScreen, std::move(split));

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
   if (mLoop and mLoop->HasQuitted())
      return false;

   mLoop->RunOnce();
   return true;
}

/// Create/Destroy GUI systems                                                
///   @param verb - the creation/destruction verb                             
void GUI::Create(Verb& verb) {
   mSystems.Create(verb);
}