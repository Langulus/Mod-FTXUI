///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "GUIItem.hpp"
#include "GUIEditor.hpp"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/loop.hpp>
#include <Langulus/Image.hpp>


///                                                                           
///   FTXUI GUI system and window interface                                   
///                                                                           
///   Manages and produces GUI items that interact with each other within an  
/// isolated system. Also acts as A::Window, since ASCII graphics are         
/// displayed in a console window, and usually there's only one associated    
/// with a process at any given time.                                         
///                                                                           
struct GUISystem final : A::UISystem, A::Window, ProducedFrom<GUI> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUI;
   LANGULUS_BASES(A::UISystem, A::Window);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created GUI items                                         
   TFactory<GUIItem> mItems;
   // An editor interface                                               
   GUIEditor* mEditor {};

   // Rendering context                                                 
   ftxui::ScreenInteractive mScreen;
   // Main loop for drawing, and reading console input                  
   ftxui::Loop* mLoop {};

   // A backbuffer that gets filled by Draw method, and is used to draw 
   // in the inner canvas                                               
   mutable ftxui::Image mBackbuffer;

public:
   GUISystem(GUI*, const Neat&);
   ~GUISystem();

   void* GetNativeHandle() const noexcept;
   Scale2 GetSize() const noexcept;
   bool IsMinimized() const noexcept;
   bool Draw(const Langulus::Ref<A::Image>&) const;

   void Create(Verb&);

   bool Update(Time);
   void Refresh();
};