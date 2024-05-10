

const loggingStore = {
    state: () => ({
        uuid: '',                      //set from App.vue
        hardware: '',                       //will be set from the LoggingStream.vue component
        exp: '',                     
        course: '',                    //set in Streams.vue from query param

       }),
       mutations:{
            SET_UUID(state, uuid){
                state.uuid = uuid;
            },
            SET_EXPERIMENT(state, exp){
                state.exp = exp;
            },
            SET_HARDWARE(state, hardware){
                state.hardware = hardware;
            },
            SET_COURSE(state, course){
                state.course = course;
            },

       },
       actions:{
            setUUID(context, uuid){
                context.commit('SET_UUID', uuid);
            },
            setExperiment(context, exp){
                context.commit('SET_EXPERIMENT', exp);
            },
            setHardware(context, hardware){
                context.commit('SET_HARDWARE', hardware);
            },
            setCourse(context, course){
                context.commit('SET_COURSE', course);
            },

       },
       getters:{
            getLogUUID(state){
                return state.uuid;
            },
            getExperiment(state){
                return state.exp
            },
            getHardware(state){
                return state.hardware
            },
            getCourse(state){
                return state.course
            }
          
       },  
  
  }

  let helpers = {
    delay(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    },
  }

  export default loggingStore;
