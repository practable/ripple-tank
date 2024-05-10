//Vue3 update

<template>
  <div class='container-fluid m-2 practable-component'>

    <div class="row"><h2>Stream Information</h2></div>

    <div class="row" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)"><p>Data URL: {{ getDataURL }}</p></div>

    <div class="row" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)"><p>Expiry time: {{ getExpiryTime }}</p></div>

    <div class="row" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)"><p>Experiment: {{ getHardware }}</p></div>

    <div class="row" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)"><p>Course: {{ getCourse }}</p></div>

      
  </div>


  </template>

<script>
import { mapGetters, mapActions } from 'vuex';

export default {

  name: 'StreamInformation',
  data () {
    return {
        

    }
  },
  components: {
    
  },
  computed:{
    ...mapGetters([
        'getDataURLObtained',
        'getDataURL',
        'getExpiryTime',
        'getCourse',
        'getHardware',
        'getStream',
        'getStreamsObtained'
        
    ]),
  },
  watch:{
    getStreamsObtained(){
      this.setHardwareCode();
    }
  },
  created(){
      
  },
  mounted(){
      

  },
  methods: {
    ...mapActions([
      'setHardware',
      'setDraggable'
    ]),
    setHardwareCode(){
      let topic = this.getStream('data').topic;
      let end_index = topic.indexOf('-')
      let hardware = topic.substr(0, end_index);
      this.setHardware(hardware);
    },



      
  },
}

</script>

<style scoped>


</style>