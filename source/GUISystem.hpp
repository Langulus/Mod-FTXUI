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
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/dom/elements.hpp>


///                                                                           
///   GUI system                                                              
///                                                                           
/// Manages and produces GUI items that interact with each other within an    
/// isolated system. Also acts as A::Window, since ASCII graphics are         
/// displayed in a console window, and usually there's only one associated    
/// with a process at any given time.                                         
///                                                                           
struct GUISystem final : A::UI::System, A::Window, ProducedFrom<GUI> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUI;
   LANGULUS_BASES(A::UI::System, A::Window);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created GUI items                                         
   TFactory<GUIItem> mItems;
   
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
   int mSplit = 30;
   int mSplitPrev = 0;

   // Selected GUISystem                                                
   std::vector<std::string> mTabNames;

   // Rendering context                                                 
   ftxui::ScreenInteractive mScreen;
   // Main loop for drawing, and reading console input                  
   ftxui::Loop* mLoop {};

public:
   GUISystem(GUI*, const Neat&);
   ~GUISystem();

   void* GetNativeHandle() const noexcept;
   Scale2 GetSize() const noexcept;
   bool IsMinimized() const noexcept;

   void Create(Verb&);

   bool Update(Time);
   void Refresh();
};