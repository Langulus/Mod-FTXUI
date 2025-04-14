///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "GUISystem.hpp"
#include "GUI.hpp"
#include <Langulus/Math/Color.hpp>
#include <ftxui/screen/color.hpp>

using namespace ftxui;


/// GUI system construction                                                   
///   @param producer - the system producer                                   
///   @param descriptor - instructions for configuring the GUI                
GUISystem::GUISystem(GUI* producer, const Many& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor}
   , mScreen      {ScreenInteractive::Fullscreen()}
   , mBackbuffer  {1, 1} {
   VERBOSE_GUI("Initializing...");

   // Create the main loop                                              
   try {
      // Create the loop and immediately yield, so that we get proper   
      // screen size and other parameters                               
      mLoop = new ftxui::Loop(&mScreen, Renderer([&] {
         LANGULUS(PROFILE);
         return image(&mBackbuffer) | flex;
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

/// First stage destruction                                                   
void GUISystem::Teardown() {
   mItems.Teardown();
}

/// Produce GUI elements in the system                                        
///   @param verb - creation verb to satisfy                                  
void GUISystem::Create(Verb& verb) {
   mItems.Create(this, verb);
}

/// System update routine                                                     
///   @param deltaTime - time between updates                                 
///   @return false if the system has been terminated by user request         
bool GUISystem::Update(Time deltaTime) {
   LANGULUS(PROFILE);
   if (mLoop and mLoop->HasQuitted())
      return false;

   // Update all UI elements                                            
   //for (auto& item : mItems)
   //   item.Update(deltaTime);

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
auto GUISystem::GetSize() const noexcept -> Scale2 {
   return {mScreen.width(), mScreen.height()};
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
   LANGULUS(PROFILE);
   const auto& image = const_cast<const A::Image&>(*what);
   auto colorData = image.GetDataList<Traits::Color>();
   auto additionalData = image.GetDataList();
   using RGB = Math::RGB;
   using Style = Logger::Emphasis;

   if (image.GetView().mWidth  != mBackbuffer.width()
   or  image.GetView().mHeight != mBackbuffer.height()) {
      mBackbuffer = Image {
         static_cast<int>(image.GetView().mWidth ),
         static_cast<int>(image.GetView().mHeight)
      };
   }

   if (colorData and *colorData and additionalData and *additionalData) {
      try {
         // A fully fledged ASCII image                                 
         // First color container is the foreground color array         
         //const auto& fgColor = (*colorData)[0].As<TMany<Math::RGBAf>>();
         //auto fgColor_raw = fgColor.GetRaw();

         // Second color container is the background color array        
         const auto& bgColor = (*colorData)[1].As<TMany<Math::RGBAf>>();
         auto bgColor_raw = bgColor.GetRaw();

         // Character data per pixel                                    
         const auto& symbols = (*additionalData)[0].As<TMany<::std::string_view>>();
         auto symbols_raw = symbols.GetRaw();

         // VT100 emphasis per pixel                                    
         //const auto& styles  = (*additionalData)[1].As<TMany<Style>>();
         //auto styles_raw = styles.GetRaw();

         // Build an ftxui::Image                                       
         auto p = mBackbuffer.get_pixels().data();
         for (uint32_t y = 0; y < image.GetView().mHeight; ++y) {
            for (uint32_t x = 0; x < image.GetView().mWidth; ++x) {
               p->style.background_color = Color {
                  static_cast<uint8_t>(bgColor_raw->r * 255),
                  static_cast<uint8_t>(bgColor_raw->g * 255),
                  static_cast<uint8_t>(bgColor_raw->b * 255)
               };

               auto fg = static_cast<uint8_t>(255 - (bgColor_raw->r * 0.299 + bgColor_raw->g * 0.587 + bgColor_raw->b * 0.114) * 255);
               if (fg >= 100 and fg <= 156) fg -= 100;
               p->style.foreground_color = Color {fg, fg, fg};

               p->grapheme = *symbols_raw;
               //p->style.background_color = Color {static_cast<uint8_t>(bgColor_raw->r * 255), 0, 0};
               //p->style.foreground_color = Color {static_cast<uint8_t>(fgColor_raw->r * 255), static_cast<uint8_t>(fgColor_raw->g * 255), static_cast<uint8_t>(fgColor_raw->b * 255)};

               /*auto& style = *styles_raw;
               p->style.blink = style & Style::Blink;
               p->style.bold = style & Style::Bold;
               p->style.dim = style & Style::Faint;
               p->style.italic = style & Style::Italic;
               p->style.inverted = style & Style::Reverse;
               p->style.underlined = style & Style::Underline;
               p->style.underlined_double = false;
               p->style.strikethrough = style & Style::Strike;
               p->style.automerge = false;*/

               //++fgColor_raw;
               ++bgColor_raw;
               ++symbols_raw;
               //++ps;
               ++p;
               //++styles_raw;
            }
         }

         return true;
      }
      catch (...) {}
   }
   
   /*if (colorData and *colorData) {
      try {
         // Only color data available                                   
         const auto& c = (*colorData)[0].As<TMany<Math::RGBAf>>(0);
         auto c_raw = c.GetRaw();

         // Build an ftxui::Image                                       
         auto p = mBackbuffer.get_pixels().data();
         for (uint32_t y = 0; y < image.GetView().mHeight; ++y) {
            for (uint32_t x = 0; x < image.GetView().mWidth; ++x) {
               p->grapheme = ' ';
               p->style.background_color = Color {static_cast<uint8_t>(c_raw->r * 255), static_cast<uint8_t>(c_raw->g * 255), static_cast<uint8_t>(c_raw->b * 255)};
               p->style.foreground_color = p->style.background_color;

               p->style.blink = false;
               p->style.bold = false;
               p->style.dim = false;
               p->style.italic = false;
               p->style.inverted = false;
               p->style.underlined = false;
               p->style.underlined_double = false;
               p->style.strikethrough = false;
               p->style.automerge = false;

               ++c_raw;
               ++p;
            }
         }

         return true;
      }
      catch (...) {}
   }*/

   return true;
}
