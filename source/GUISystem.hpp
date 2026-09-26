///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "GUIItem.hpp"
#include "GUIEditor.hpp"
#include <Langulus/Factory.hpp>
#include <Langulus/Image.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/screen/surface.hpp>

struct ftxuisurf : ftxui::Surface {
   auto get_pixels() -> std::vector<ftxui::Cell>& {
      return cells_;
   }
};

///                                                                           
///   FTXUI GUI system and window interface                                   
///                                                                           
///   Manages and produces GUI items that interact with each other within an  
/// isolated system. Also acts as A::Window, since ASCII graphics are         
/// displayed in a console window, and usually there's only one associated    
/// with a process at any given time.                                         
///                                                                           
struct GUISystem : Things::UISystem, Things::Window, Flow::ProducedFrom<GUI> {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = GUI;
   using CTTI_Bases     = Types<Things::UISystem, Things::Window>;
   using CTTI_Ability   = Verbs::Create;

private:
   // List of created GUI items                                         
   TFactory<GUIItem> mItems;
   // An editor interface                                               
   GUIEditor* mEditor {};

   // Rendering context                                                 
   ftxui::ScreenInteractive mScreen;
   // Main loop for drawing, and reading console input                  
   ftxui::Loop* mLoop {};

   // A backbuffer that gets filled by the renderer module              
   ftxui::Surface& mBackbuffer;

public:
   GUISystem(GUI*, const Many&);
   ~GUISystem();

   void Create(Verb&);

   void* GetNativeHandle() const noexcept;
   auto GetSize() const noexcept -> Scale2;
   bool IsMinimized() const noexcept;
   bool Draw(const Ref<Things::Image>&) const;
   bool Update(Time);
   void Refresh();
   void Teardown();
};