///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "GUISystem.hpp"
#include <Flow/Verbs/Create.hpp>
#include <ftxui/component/component.hpp>


///                                                                           
///   User interface module using FTXUI                                       
///                                                                           
/// Manages and produces GUI systems                                          
///                                                                           
struct GUI final : A::UI::Module {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::UI::Module);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created GUI systems                                       
   // Each system will appear as a tab on the top of the window         
   TFactory<GUISystem> mSystems;

public:
   GUI(Runtime*, const Neat&);

   bool Update(Time);
   void Create(Verb&);
};

