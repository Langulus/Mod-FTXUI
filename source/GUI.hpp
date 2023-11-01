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
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/util/ref.hpp>


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
   // The main terminal renderer and loop                               
   ::std::string mLog = "<log not attached>";
   ::std::string mInput;
   ftxui::Component mCommand, mTabs, mTree, mRenderer;
   ftxui::ScreenInteractive mScreen;
   ftxui::Loop* mLoop {};
   int mSplit = 50;

   // Selected GUISystem                                                
   int mSelectedTab = 0;
   std::vector<std::string> mTabNames;
   int mSelectedTree = 0;
   std::vector<std::string> mTreeNames;

   // List of created GUI systems                                       
   // Each system will appear as a tab on the top of the window         
   TFactory<GUISystem> mSystems;

public:
   GUI(Runtime*, const Neat&);
   ~GUI();

   bool Update(Time);
   void Create(Verb&);
};

