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
#include <ftxui/screen/color.hpp>

using namespace ftxui;


/// GUI system construction                                                   
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the GUI                
GUISystem::GUISystem(GUI* producer, const Neat& descriptor)
   : Resolvable {this}
   , ProducedFrom {producer, descriptor}
   , mItems {this}
   , mScreen {ScreenInteractive::Fullscreen()} { 
   VERBOSE_GUI("Initializing...");

   // Create the main loop                                              
   auto component = Renderer([&] {
      auto my_image = canvas([&](Canvas& c) {
         c.DrawPointLine(0, 0, c.width(), c.height());

         if (mBackbuffer) {
            auto raw = mBackbuffer.GetRaw();
            ftxui::Canvas::Stylizer style = [](ftxui::Pixel& p) {
               p.background_color = ftxui::Color::Red;
            };

            int x = 0, y = 0;
            while (*raw) {
               auto end = raw + 1;
               while (*end != '\0' and *end != '\n')
                  ++end;

               const auto count = end - raw;
               c.DrawText(0, y*4, std::string(std::string_view(raw, count)), style);
               if (*end == '\n')
                  ++y;
               raw += count + 1;
            }
            //c.DrawText(0, 0, "width = " + std::to_string(c.width()));
            //c.DrawText(0, 4, "height = " + std::to_string(c.height()));
         }
      });
      return my_image | flex;
   });

   try {
      // Create the loop and immediately yield, so that we get proper   
      // screen size and other parameters                               
      mLoop = new ftxui::Loop(&mScreen, component);
      mLoop->RunOnce();
   }
   catch (const std::exception& e) {
      Logger::Error(Self(), "Unable to create FTXUI main loop: ", e.what());
      throw;
   }

   Couple(descriptor);
   VERBOSE_GUI("Initialized");
}

/// Shutdown the module                                                       
GUISystem::~GUISystem() {
   if (mEditor)
      delete mEditor;
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

/// Get console window's handle                                               
///   @return the handle                                                      
void* GUISystem::GetNativeHandle() const noexcept {
   // No use for a console window handle                                
   return nullptr;
}

/// Get the console window size, in characters                                
///   @return the size of the console window, in characters                   
Math::Scale2 GUISystem::GetSize() const noexcept {
   return {mScreen.dimx(), mScreen.dimy()};
}

/// Check if console window is minimized                                      
///   @return always false                                                    
bool GUISystem::IsMinimized() const noexcept {
   // Always assume console window is not minimized                     
   return false;
}

/// Draw any string, including VT100 encoded colors                           
///   @attention contents must be null-terminated                             
///   @return true if arguments contains ASCII stuff                          
bool GUISystem::Draw(const Any& what) const {
   try { mBackbuffer = what.As<Text>(); } 
   catch (...) { return false; }
   return true;
}
