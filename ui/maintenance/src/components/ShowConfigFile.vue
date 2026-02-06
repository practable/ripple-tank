//Vue3 update

<template>
  <div class='container-fluid m-2 practable-component table'>

    <div class="row "><h2>Config JSON</h2></div>

    <div v-if="getConfigJSON != ''">{{ getConfigJSON }}</div>
    <div v-else>No config file available</div>

      
  </div>


  </template>

<script>
import { mapGetters, mapActions } from 'vuex';

export default {

  name: 'ShowConfigFile',
  data () {
    return {
        

    }
  },
  components: {
    
  },
  computed:{
    ...mapGetters([
        'getConfigJSON',

        
    ]),
  },
  watch:{
    
  },
  created(){
      
  },
  mounted(){
      

  },
  methods: {
    ...mapActions([
      
    ]),
    



      
  },
}

</script>

<style scoped>


</style>