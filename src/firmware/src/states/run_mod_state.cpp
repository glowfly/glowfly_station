#ifndef RUNMODSTATE_H
#define RUNMODSTATE_H

#include <mappings.hpp>
#include <visualization/script_context.hpp>
#include <visualization/frequency_analyzer.hpp>
#include <network/websocket/commands/analyzer_command.hpp>
#include <network/websocket/commands/server_commands.hpp>

#include "state.hpp"
#include "states/invalid_mod_state.cpp"
#include "station_context.hpp"
#include "views/run_mod_view.cpp"

namespace SyncBlink
{
    class RunModState : public State
    {
        public:
            RunModState(StationContext& context, std::shared_ptr<ScriptContext> scriptContext) 
                : _context(context), _scriptContext(scriptContext)
            {
                _socketEventHandleId = context.getSocketServer()
                    .serverCommandEvents
                    .addEventHandler([this](Server::Command command) 
                    { 
                        handleExternalSource(command); 
                    });
                _modEventHandleId = context.getModManager()
                    .activeModChangedEvents
                    .addEventHandler([this]() 
                    {
                        _activeModChanged = true;
                    });
                _runModView = std::make_shared<RunModView>();
                _modName = _scriptContext->getModName();
            }

            ~RunModState()
            {
                _context.getSocketServer()
                    .serverCommandEvents
                    .removeEventHandler(_socketEventHandleId);
                _context.getModManager()
                    .activeModChangedEvents
                    .removeEventHandler(_modEventHandleId);
            }

            void run(StationContext& context)
            {                         
                context.getDisplay().setView(_runModView);
                context.getDisplay().setLeftStatus(_modName);
                
                if(_activeModChanged) context.resetState();
                else handleMicrophoneSource(context.getSocketServer());
            }

        private:
            void handleMicrophoneSource(SocketServer& socketServer)
            {   
                if(checkScriptContext() && _scriptContext->source == AnalyzerSource::Station)
                {
                    AnalyzerResult result = _frequencyAnalyzer.loop();
                    Client::Command command = { millis(), Client::ANALYZER_UPDATE };
                    command.analyzerCommand = result.ToCommand();

                    socketServer.broadcast(command);
                    setView(command.analyzerCommand);

                    uint32_t delta = millis() - _lastLedUpdate;
                    _lastLedUpdate = millis();

                    _scriptContext->updateAnalyzerResult(result.volume, result.dominantFrequency);
                    _scriptContext->run(delta);
                }
            }

            void handleExternalSource(Server::Command& serverCommand)
            {
                if(checkScriptContext()
                && _scriptContext->source != AnalyzerSource::Station
                && serverCommand.commandType == Server::CommandType::EXTERNAL_ANALYZER)
                {
                    Client::Command command = { millis(), Client::ANALYZER_UPDATE };
                    command.analyzerCommand = serverCommand.analyzerCommand;

                    _context.getSocketServer().broadcast(command);
                    setView(command.analyzerCommand);

                    uint32_t delta = millis() - _lastLedUpdate;
                    _lastLedUpdate = millis();

                    _scriptContext->updateAnalyzerResult(command.analyzerCommand.volume, command.analyzerCommand.frequency);
                    _scriptContext->run(delta);
                }
            }

            bool checkScriptContext()
            {
                bool valid = true;
                if(_scriptContext->isFaulted())
                {
                    _context.currentState = std::make_shared<InvalidModState>();
                    valid = false;
                }
                return valid;
            }

            void setView(AnalyzerCommand& command)
            {
                _runModView->volume = command.volume;
                _runModView->decibel = command.decibel;
                if(command.volume > 0 && command.frequency > 0)
                {
                    _runModView->dominantFrequency = command.frequency;
                    _runModView->setFreqBars(command.freqBins);
                }
                else
                {
                    _runModView->dominantFrequency = 0;
                    _runModView->fadeFrequencyRange();
                }
            }

            StationContext& _context;
            std::shared_ptr<ScriptContext> _scriptContext;
            std::shared_ptr<RunModView> _runModView;
            uint64_t _socketEventHandleId = 0, _modEventHandleId = 0;
            std::string _modName;
            bool _activeModChanged = false;

            FrequencyAnalyzer _frequencyAnalyzer;
            uint64_t _lastLedUpdate = millis();
    };
}

#endif // RUNMODSTATE_H