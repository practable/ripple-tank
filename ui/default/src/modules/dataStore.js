//import Vue from 'vue';
//{"timestamp":529673373,"payload":{"state":"STATE_WAIT","step":{"hz":0,"rpm":0},"encode":{"hz":"    0.000","rpm":"    0.002","pos(raw)":19599,"pos":["  215.321","  215.321","  215.321","  215.321"]},"meta":{"samples":4,"time":[529673223,529673273,529673323,529673373]}}}

const dataStore = {
   state: () => ({
      //current values
      start_time: 0,
      currentTime: [0],
      wavelength: 0,
      frequency: 0,

      //recorded data
      data: [],
      data_set_index: 0,
      max_data_points: 5000,
      isRecording: false,
      x_axis_variable: 'frequency',
      y_axis_variable: 'wavelength', 
   }),
   mutations:{
      SET_IS_RECORDING(state, set){
         state.isRecording = set;
      },
      SET_DATASET_INDEX(state, set){
         state.data_set_index = set;
      },
      ADD_TO_DATASET_INDEX(state){
         state.data_set_index += 1;
      },
      CLEAR_ALL_DATA(state){
         state.data = [];
      },
      ADD_DATA(state, object){
         state.data.push(object);
      },
      DELETE_DATA(state, id){
         state.data.splice(id, 1);
      },
      SET_START_TIME(state, time){
         state.start_time = time;
      },
      SET_CURRENT_TIME(state, time){
         state.currentTime = time;
      },
      SET_CURRENT_WAVELENGTH(state, l){
         state.wavelength = l;
      },
      SET_CURRENT_FREQUENCY(state, f){
         state.frequency = f;
      },
      SET_X_AXIS(state, variable){
         state.x_axis_variable = variable;
      },
      SET_Y_AXIS(state, variable){
         state.y_axis_variable = variable;
      }
   },
   actions:{
      setIsRecording(context, value){
         context.commit('SET_IS_RECORDING', value);
      },
      setDatasetIndex(context, set){
         context.commit('SET_DATASET_INDEX', set);
      },
      addToDatasetIndex(context){
         context.commit('ADD_TO_DATASET_INDEX');
      },
      clearAllData(context){
         context.commit('CLEAR_ALL_DATA');
      },
      addData(context,object){
         context.commit('ADD_DATA', object);
      },
      deleteData (context, dataId) {
         context.commit('DELETE_DATA', dataId);
      },
      setStartTime(context, time){
         context.commit("SET_START_TIME", time);
      },
      setCurrentTime(context, time){
         context.commit('SET_CURRENT_TIME', time);
      },
      setCurrentWavelength(context, l){
         context.commit('SET_CURRENT_WAVELENGTH', l);
      },
      setCurrentFrequency(context, f){
         context.commit('SET_CURRENT_FREQUENCY', f);
      },
      setXAxis(context, variable){
         context.commit('SET_X_AXIS', variable);
      },
      setYAxis(context, variable){
         context.commit('SET_Y_AXIS', variable);
      }
   },
   getters:{
      getData(state){
         return state.data;
      },
      getIsRecording(state){
         return state.isRecording;
      },
      getNumData(state){
         return state.data.length;
     },
     getMaxDataPoints(state){
      return state.max_data_points;
   },
     getMaxReached(state){
      if(state.data.length >= state.max_data_points){
         return true;
      } else {
         return false;
      }
   },
     getDataSets(state){
         let datasets = helpers.GetDataSets(state.data);
         return datasets;
     },
     getDatasetIndex(state){
      return state.data_set_index;
      },
      getLatestDatasetIndex(state){
         let latest_data = state.data[state.data.length - 1];
         let index = latest_data.set;
         return index;
      },
     getStartTime(state){
      return state.start_time;
     },
     getCurrentTime(state){
      return state.currentTime;
     },
      getSecondsSinceStart(state){ 
         let new_time = []
         for(let i=0;i<state.currentTime.length; i++){
            if(state.currentTime[i] - state.start_time >= 0){
               new_time.push((state.currentTime[i] - state.start_time)/1000);  //in seconds
            } else{
               state.start_time = state.currentTime[i];
               new_time.push(0.0);
            }
         }
         return new_time;
      },
     getCurrentWavelength(state){
      return state.wavelength;
     },
     getCurrentFrequency(state){
      return state.frequency;
     },
     getXAxisVariable(state){
         return state.x_axis_variable;
      },
      getYAxisVariable(state){
         return state.y_axis_variable;
      },
   }
      
}

export const helpers = {
   GetDataSets(data) {
      let datasets = [];   //array of arrays storing each separate dataset
      let next_dataset = [];
      let current_index = 0;
      data.forEach(d => {

         if(d.set == current_index + 1){
            datasets.push(next_dataset);
            current_index += 1;

            next_dataset = [];
         }

         next_dataset.push(d);
      })

      if(next_dataset.length > 0){
         datasets.push(next_dataset);
      }
      
      return datasets;
   }
}

export default dataStore