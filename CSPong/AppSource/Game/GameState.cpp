//
//  GameState.cpp
//  CSPong
//  Created by Scott Downie on 30/06/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <Game/GameState.h>

#include <Common/TransitionSystem.h>
#include <Game/GameEntityFactory.h>
#include <Game/Ball/BallControllerComponent.h>
#include <Game/Physics/PhysicsSystem.h>
#include <Game/Particles/ParticleEffectComponentFactory.h>

#include <ChilliSource/Core/Base.h>
#include <ChilliSource/Core/Delegate.h>
#include <ChilliSource/Core/Scene.h>
#include <ChilliSource/Rendering/Camera.h>
#include <ChilliSource/Rendering/Lighting.h>

namespace CSPong
{
    namespace
    {
        const u32 k_targetScore = 5;
    }
    //------------------------------------------------------
    //------------------------------------------------------
    void GameState::CreateSystems()
    {
        m_transitionSystem = CreateSystem<TransitionSystem>(1.0f, 1.0f);
        m_physicsSystem = CreateSystem<PhysicsSystem>();
        m_scoringSystem = CreateSystem<ScoringSystem>();
        m_gameEntityFactory = CreateSystem<GameEntityFactory>(m_physicsSystem, m_scoringSystem);
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GameState::OnInit()
    {   
		const s32 numParticles = 1;
		ParticleEffectComponentFactory::ParticleType ballParticleTypes
		{ 
			ParticleEffectComponentFactory::ParticleType::k_smokeStream 
		};
		auto particleECF = CSCore::Application::Get()->GetSystem<ParticleEffectComponentFactory>();
		particleECF->AssignBallParticles(&ballParticleTypes, numParticles);

        GetScene()->SetClearColour(CSCore::Colour::k_black);
        
        CSCore::EntitySPtr camera = m_gameEntityFactory->CreateCamera();
        GetScene()->Add(camera);

        CSCore::EntitySPtr ambientLight = m_gameEntityFactory->CreateAmbientLight();
        GetScene()->Add(ambientLight);
        
        CSCore::EntitySPtr diffuseLight = m_gameEntityFactory->CreateDiffuseLight();
        GetScene()->Add(diffuseLight);
        
        CSCore::EntitySPtr arena = m_gameEntityFactory->CreateArena();
        GetScene()->Add(arena);
        
        m_ball = m_gameEntityFactory->CreateBall();
        GetScene()->Add(m_ball);
        
        m_scoreChangedConnection = m_scoringSystem->GetScoreChangedEvent().OpenConnection(CSCore::MakeDelegate(this, &GameState::OnGoalScored));

        m_transitionInConnection = m_transitionSystem->GetTransitionInFinishedEvent().OpenConnection([=]()
        {
            m_ball->GetComponent<BallControllerComponent>()->Activate();
        });
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GameState::OnGoalScored(const ScoringSystem::Scores& in_scores)
    {
        /*BallControllerComponentSPtr ballController = m_ball->GetComponent<BallControllerComponent>();
        ballController->Deactivate();
        
        /*if(in_scores[0] >= k_targetScore)
        {
            
        }
        else if(in_scores[1] >= k_targetScore)
        {
            
        }
        else
        {
            
        }*/
    }
    //------------------------------------------------------------
    //------------------------------------------------------------
    void GameState::OnDestroy()
    {
        m_scoreChangedConnection.reset();


		// update all profiles
		PROFILE_UPDATE();

		// create directory for profiling (won't create it if it already exists)
		bool isCreated = CSCore::Application::Get()->GetFileSystem()->CreateDirectoryPath(CSCore::StorageLocation::k_saveData, "Profile");

		if (isCreated)
		{
			// get both parts of profile string
			std::string profileStr = PROFILE_GET_FLAT_STRING();
			profileStr.append("\r\n\r\n");
			profileStr.append(PROFILE_GET_TREE_STRING());

			// write profile 
			bool isWritten = CSCore::Application::Get()->GetFileSystem()->WriteFile(CSCore::StorageLocation::k_saveData, "Profile/cspong_profile.txt", profileStr);

			if (!isWritten)
			{
				CS_LOG_ERROR("PROFILING OUTPUT: Failed to write profile file to SaveData/Profile/cspong_profile.txt");
			}
			else
			{
				CS_LOG_VERBOSE("PROFILING OUTPUT: Saved profile file to SaveData/Profile/cspong_profile.txt");
			}
		}
		else
		{
			CS_LOG_ERROR("PROFILING OUTPUT: Failed to create directory path for SaveData/Profile");
		}
    }
}