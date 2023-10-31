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
#include "GUIFont.hpp"


///                                                                           
///   GUI system                                                              
///                                                                           
/// Manages and produces GUI items that interact with each other within an    
/// isolated system                                                           
///                                                                           
struct GUISystem final : A::UI::System, ProducedFrom<GUI> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUI;
   LANGULUS_BASES(A::UI::System);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created GUI items                                         
   TFactory<GUIItem> mItems;
   TFactoryUnique<GUIFont> mFonts;

public:
   GUISystem(GUI*, const Neat&);
   ~GUISystem();

   void Create(Verb&);
   void Draw(Verb&);

   void Refresh();
};