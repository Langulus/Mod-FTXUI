///                                                                           
/// Langulus::Module::FTXUI                                                   
/// Copyright (c) 2023 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>


///                                                                           
///   GUI item                                                                
///                                                                           
/// A single widget inside of a GUI system                                    
///                                                                           
struct GUIEditor final : A::UIUnit, ProducedFrom<GUISystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) GUISystem;
   LANGULUS_BASES(A::UIUnit);

private:
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
   ftxui::Component mMain;
   int mSplit = 30;
   int mSplitPrev = 0;

   // Selected GUISystem                                                
   std::vector<std::string> mTabNames;

public:
   GUIEditor(GUISystem*, const Neat&);

   virtual void Update(Time) {}
   void Refresh();
};

