//Store for sending commands through the dataSocket


const commandStore = {
    state: () => ({
        dataSocket: null,
        currentMode: 'stop',     //'start', 'pulse'
        currentState: 'STATE_WAIT',      //reported state from hardware, do I need this in the UI?
        driver: {
            'hz': 1, 
            'from_hardware_hz': 1, 
            'max':100, 
            'min':1, 
            'step':1,       //for the input slider only, buttons can make the step smaller
            'pulse': 100,    //pulse time in ms, only applied in pulse mode
            'pulse_min': 1,
            'pulse_max': 1000,   //ms
            'pulse_step': 10    //ms, for slider input only
        },   
       }),
       mutations:{
        SET_DATA_SOCKET(state, socket){
            state.dataSocket = socket;
        },
        SET_STOP(state){
            state.currentMode = 'stop';
        },
        SET_START(state){
            state.currentMode = 'start';
        },
        SET_PULSE(state){
            state.currentMode = 'pulse';
        },
        COMMAND_STOP(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "stop"
                }));
            }
        },
        COMMAND_START(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "start"
                }));
            }
        },
        COMMAND_PULSE(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "pulse",
                    to: state.driver.pulse
                }));
            }
        },
        UPDATE_DRIVING_FREQUENCY(state, val){
            state.driver.hz = Number(val);
        },
        UPDATE_PULSE_LENGTH(state, val){
            state.driver.pulse = Number(val);
        },
        COMMAND_UPDATE_DRIVING_FREQUENCY(state){
            if(state.dataSocket != null){
                state.dataSocket.send(JSON.stringify({
                    set: "hz",
                    to: state.driver.hz
                }));
            }
        },
        SET_REPORTED_DRIVING_FREQUENCY(state, val){
            state.driver['from_hardware_hz'] = val;
        },

       },
       actions:{
        setDataSocket(context, socket){
            context.commit("SET_DATA_SOCKET", socket);
        },
        setModeStop(context){
            context.commit('SET_STOP');
        },
        setModeStart(context){
            context.commit('SET_START');
        },
        setModePulse(context){
            context.commit('SET_PULSE');
        },
        sendCommandStop(context){
            context.commit('COMMAND_STOP');
        },
        sendCommandStart(context){
            context.commit('COMMAND_START');
        },
        sendCommandPulse(context){
            context.commit('COMMAND_PULSE');
        },
        updateDrivingFrequency(context, val){
            let to_set = val;
            if(val < context.getters.getDrivingFrequencyMin){
                to_set = context.getters.getDrivingFrequencyMin;
            } else if(val > context.getters.getDrivingFrequencyMax){
                to_set = context.getters.getDrivingFrequencyMax;
            }
            context.commit('UPDATE_DRIVING_FREQUENCY', to_set);
        },
        updatePulseLength(context, val){
            let to_set = val;
            if(val < context.getters.getPulseLengthMin){
                to_set = context.getters.getPulseLengthMin;
            } else if(val > context.getters.getPulseLengthMax){
                to_set = context.getters.getPulseLengthMax;
            }
            context.commit('UPDATE_PULSE_LENGTH', to_set);
        },
        sendCommandUpdateDrivingFrequency(context){
            context.commit('COMMAND_UPDATE_DRIVING_FREQUENCY');
        },
        setReportedDrivingFrequency(context, val_object){
            context.commit('SET_REPORTED_DRIVING_FREQUENCY', val_object);
        },
        

       },
       getters:{
        getDataSocket(state){
            return state.dataSocket;
        },
        getCurrentMode(state){
            return state.currentMode;
        },
        getDrivingFrequencyMin(state){
            return state.driver.min;
        },
        getDrivingFrequencyMax(state){
            return state.driver.max;
        },
        getDrivingFrequencyStep(state){
            return state.driver.step;
        },
        getDrivingFrequency(state){
            return state.driver.hz;
        },
        getReportedDrivingFrequencyHz(state){
            return state.driver['from_hardware'].hz;
        },
        getPulseLength(state){
            return state.driver.pulse;
        },
        getPulseLengthMin(state){
            return state.driver['pulse_min'];
        },
        getPulseLengthMax(state){
            return state.driver['pulse_max'];
        },
        getPulseLengthStep(state){
            return state.driver['pulse_step'];
        },
          
       },  
  
  }

  export default commandStore;