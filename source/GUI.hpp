///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "GUISystem.hpp"
#include <Langulus/Verbs/Create.hpp>
#include <ftxui/component/component.hpp>


///                                                                           
///   User interface module using FTXUI                                       
///                                                                           
/// Manages and produces GUI systems (which are also console windows)         
///                                                                           
struct GUI final : Things::UIModule, Things::PlatformModule {
   using CTTI_Abstract  = No;
   using CTTI_Bases     = Types<Things::UIModule, Things::PlatformModule>;
   using CTTI_Ability   = Verbs::Create;

private:
   // List of created GUI systems                                       
   // Each system will appear as a tab on the top of the window         
   TFactory<GUISystem> mSystems;

public:
   GUI(Things::Runtime*, const Many&);

   bool Update(Time);
   void Create(Verb&);
   void Teardown();
};

