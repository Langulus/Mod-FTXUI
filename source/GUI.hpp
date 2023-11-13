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
   // List of created GUI systems                                       
   // Each system will appear as a tab on the top of the window         
   TFactory<GUISystem> mSystems;

   // The main terminal renderer and loop                               
   ::std::vector<ftxui::Element> mLog;

   ftxui::Component mTabSelector;
   ftxui::Component mTabContents;
   int mSelectedTab = 0;

   ftxui::Component mLogTab;
   int mLogScroll;

   ftxui::Component mFlowTab;
   ftxui::Component mFlowContents;
   ftxui::Component mFlowCommand;
   ::std::string    mFlowCommandInput;

   ftxui::Component mLeftPanel;

   ftxui::Component mRightPanel;
   ftxui::Component mTree;
   ftxui::Component mSelection;

   ftxui::Component mRenderer;
   ftxui::ScreenInteractive mScreen;
   ftxui::Loop* mLoop {};
   int mSplit = 30;
   int mSplitPrev = 0;

   // Selected GUISystem                                                
   std::vector<std::string> mTabNames;

public:
   GUI(Runtime*, const Neat&);
   ~GUI();

   bool Update(Time);
   void Create(Verb&);
};

