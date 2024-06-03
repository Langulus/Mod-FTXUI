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
#include <Math/Color.hpp>
#include <ftxui/screen/color.hpp>

using namespace ftxui;


/// GUI system construction                                                   
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the GUI                
GUISystem::GUISystem(GUI* producer, const Neat& descriptor)
   : Resolvable {this}
   , ProducedFrom {producer, descriptor}
   , mItems {this}
   , mScreen {ScreenInteractive::Fullscreen()}
   , mBackbuffer {1, 1} {
   VERBOSE_GUI("Initializing...");

   // Create the main loop                                              
   try {
      // Create the loop and immediately yield, so that we get proper   
      // screen size and other parameters                               
      mLoop = new ftxui::Loop(&mScreen, Renderer([&] {
         return canvas([&](Canvas& c) {
            c.DrawImage(0, 0, mBackbuffer);
         }) | flex;
      }) | CatchEvent([&](Event event) -> bool {
         //if (event.is_mouse())
            //Logger::Special("mouse event"); //this works, but useful only for keyboard
         return false;
      }));
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
   mScreen.PostEvent(Event::Custom);
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

/// Draw an image, interpreting it as console output                          
///   @param what - the image to interpret to console output                  
///   @return true if interpretation was a success                            
bool GUISystem::Draw(const Langulus::Ref<A::Image>& what) const {
   const auto& image = const_cast<const A::Image&>(*what);
   auto colorData = image.GetDataList<Traits::Color>();
   auto additionalData = image.GetDataList();
   using RGB = Math::RGB;
   using Style = Logger::Emphasis;

   mBackbuffer = Image {
      static_cast<int>(image.GetView().mWidth),
      static_cast<int>(image.GetView().mHeight)
   };

   if (colorData and *colorData and additionalData and *additionalData) {
      try {
         // A fully fledged ASCII image                                 
         // First color container is the foreground color array         
         auto& fgColor = (*colorData)[0].As<TMany<RGB>>();
         // Second color container is the background color array        
         auto& bgColor = (*colorData)[1].As<TMany<RGB>>();
         // Character data per pixel                                    
         auto& symbols = (*additionalData)[0].As<TMany<Text>>();
         // VT100 emphasis per pixel                                    
         auto& styles  = (*additionalData)[1].As<TMany<Style>>();

         // Build an ftxui::Image                                       
         for (uint32_t y = 0; y < image.GetView().mHeight; ++y) {
            for (uint32_t x = 0; x < image.GetView().mWidth; ++x) {
               uint32_t i = y * image.GetView().mWidth + x;
               Pixel& p = mBackbuffer.PixelAt(x, y);
               p.character = symbols[i];
               p.background_color = Color {bgColor[i].r, bgColor[i].g, bgColor[i].b};
               p.foreground_color = Color {fgColor[i].r, fgColor[i].g, fgColor[i].b};

               auto& style = styles[i];
               p.blink = style & Style::Blink;
               p.bold = style & Style::Bold;
               p.dim = style & Style::Faint;
               p.inverted = style & Style::Reverse;
               p.underlined = style & Style::Underline;
               p.underlined_double = false;
               p.strikethrough = style & Style::Strike;
               p.automerge = false;
            }
         }

         return true;
      }
      catch (...) {}
   }
   
   if (colorData and *colorData) {
      try {
         // Only color data available                                   
         auto& c = (*colorData)[0].As<TMany<RGB>>(0);

         // Build an ftxui::Image                                       
         for (uint32_t y = 0; y < image.GetView().mHeight; ++y) {
            for (uint32_t x = 0; x < image.GetView().mWidth; ++x) {
               uint32_t i = y * image.GetView().mWidth + x;
               Pixel& p = mBackbuffer.PixelAt(x, y);
               p.character = " ";
               p.background_color = Color {c[i].r, c[i].g, c[i].b};
               p.foreground_color = p.background_color;

               p.blink = false;
               p.bold = false;
               p.dim = false;
               p.inverted = false;
               p.underlined = false;
               p.underlined_double = false;
               p.strikethrough = false;
               p.automerge = false;
            }
         }

         return true;
      }
      catch (...) {}
   }

   return true;
}
