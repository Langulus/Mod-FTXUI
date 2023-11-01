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
   mTreeNames = {"#ROOT", "#some other thing"};
   mTree = Menu(&mTreeNames, &mSelectedTree);

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
      return mTree->Render();
   });
   auto split = ResizableSplitRight(right, left, &mSplit);

   // Create the main loop                                              
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