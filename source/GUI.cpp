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


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
GUI::GUI(Runtime* runtime, const Neat&)
   : A::UI::Module {MetaOf<GUI>(), runtime}
   , mSystems {this}
   , mScreen {ftxui::ScreenInteractive::Fullscreen()} {
   Logger::Verbose(Self(), "Initializing...");
   mCommand   = ftxui::Input(&mInput, " -input here- ");
   mComponent = ftxui::Container::Vertical({mCommand});
   mRenderer  = ftxui::Renderer(mComponent, [&] {
      return ftxui::vbox({
         ftxui::text("Hello " + mInput),
         ftxui::filler(),
         ftxui::separator(),
         ftxui::hbox(ftxui::text("> "), mCommand->Render())
      }) | ftxui::border;
   });
   mLoop = new ftxui::Loop(&mScreen, std::move(mRenderer));
   Logger::Verbose(Self(), "Initialized");
}

GUI::~GUI() {
   if (mLoop)
      delete mLoop;
}

/// Module update routine                                                     
///   @param dt - time from last update                                       
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