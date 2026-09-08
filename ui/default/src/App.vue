<template>
  <div id="app" class='container-fluid-sm m-0'>
       <navigation-bar @toggleworkspace="addWorkspace" @clearworkspace="clearWorkspace" @addruler="rulerAdded = true" @addprotractor="protractorAdded = true"/>

        <streams id='streams' />
        <data-stream id="data-stream" />

        <div v-if="isWorkspaceOn">
          <workspace :protractorAdded="protractorAdded" :rulerAdded="rulerAdded"/>
        </div>

      
        <div :class="isMobile ? 'd-flex flex-column' : 'row'" id='component-grid'>
        <!-- first-row etc only exist as styles when large screen -->
            <div :class="isMobile ? '' : 'd-flex'" id='first-row'>
              <div :class="isMobile ? 'drop-area drop-area-mobile' : 'drop-area drop-area-two-fifths'" id='drop_0_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><webcam-stream id='webcam-stream' /></div>
              <div :class="isMobile ? 'drop-area drop-area-mobile' : 'drop-area drop-area-three-fifths'" id='drop_0_1' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><image-capture id='image-capture' /></div>
            </div>

            <div :class="isMobile ? '' : 'd-flex'" id='second-row'>
              <div :class="isMobile ? 'drop-area drop-area-mobile' : 'drop-area drop-area-full'" id='drop_1_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><control-panel id="control-panel" /></div>
            </div>

            <div :class="isMobile ? '' : 'd-flex'" id='third-row'>
              <div :class="isMobile ? 'drop-area drop-area-mobile' : 'drop-area drop-area-half'" id='drop_2_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><snapshot-recorder id="snapshot-recorder" :headings="['Frequency \n[Hz]', 'Wavelength\n[cm]', 'Wave speed\n[cm/s]']"/></div>
              <div :class="isMobile ? 'drop-area drop-area-mobile' : 'drop-area drop-area-half'" id='drop_2_1' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><graph id="graph"/></div>
            </div>

            <div :class="isMobile ? '' : 'd-flex'" id='fourth-row'>
              <div :class="isMobile ? 'drop-area drop-area-mobile' : 'drop-area drop-area-half'" id='drop_3_0' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"><video-capture id='video-capture' /></div>
              <div :class="isMobile ? 'drop-area drop-area-mobile' : 'drop-area drop-area-half'" id='drop_3_1' :draggable='getDraggable' @dragstart="dragComponent" @drop='dropComponent' @dragover.prevent @dragenter='dragEnter' @dragleave="dragLeave"></div>
            </div>
        </div>
     
   
  </div>
</template>

<script>
import { mapGetters, mapActions } from 'vuex';

import ImageCapture from "./components/ImageCapture.vue";
import VideoCapture from "./components/VideoCapture.vue";
import WebcamStream from "./components/WebcamStream.vue";
import DataStream from "./components/DataStream.vue";
import NavigationBar from "./components/NavigationBar.vue";
import Streams from "./components/Streams.vue";
import ControlPanel from "./components/ControlPanel.vue"
import StreamInformation from "./components/StreamInformation.vue";
import ShowConfigFile from "./components/ShowConfigFile.vue";
import Workspace from "./components/Workspace.vue";
import SnapshotRecorder from './components/SnapshotRecorder.vue';
import Graph from './components/Graph.vue';

export default {
  name: 'App',
  components: {
    Streams,
    WebcamStream,
    DataStream,
    NavigationBar,
    ControlPanel,
    StreamInformation,
    ShowConfigFile,
    VideoCapture,
    ImageCapture,
    Workspace,
    SnapshotRecorder,
    Graph

  },
  data() {
    return {
      // windowWidth: window.innerWidth,
      isWorkspaceOn: false,
      protractorAdded: false,
      rulerAdded: false,
    }
  },
  created(){
    this.$store.dispatch('setUsesLocalStorage', this.hasStorage());
  },
  mounted(){
    this.updateUUID();

    window.onresize = () => {this.setWindowWidth(window.innerWidth)};
  },
  computed:{
    ...mapGetters([
      'getDraggable',
      'getUsesLocalStorage',
      'isMobile'
    ]),
  },
  watch: {
    isMobile() {
      this.$nextTick(() => {
        document.querySelectorAll('#component-grid > div, .drop-area')
        .forEach((el) => {
          el.style.width = '';
          el.style.height = '';
        });
      });
    },
  },
  methods:{
    ...mapActions([
        'setWindowWidth'
    ]),
    clearWorkspace(){
      this.isWorkspaceOn = false;
      this.protractorAdded = false;
      this.rulerAdded = false;
    },
    addWorkspace(){
        this.isWorkspaceOn = true;
    },
    dragComponent(event){
        event.dataTransfer.effectAllowed = 'move';
         //console.log("Dragged event: ");
         //console.log(event);
         let element = event.target;
         if(element.classList.contains('drop-area')){
           if(element.childNodes[0] != null){
              event.dataTransfer.setData("text/html", element.id + "|" + element.childNodes[0].id);
           } else{
              event.dataTransfer.setData("text/html", element.id + "|" + 'empty');
           }
            
         } else{
           while(element.parentNode){
              element = element.parentNode;
              //console.log(element.id);
              if(element.classList.contains('drop-area')){
                if(element.childNodes[0] != null){
                  event.dataTransfer.setData("text/html", element.id + "|" + element.childNodes[0].id);
              } else{
                  event.dataTransfer.setData("text/html", element.id + "|" + 'empty');
              }
                break;
              }
            }
         }
    },
    dropComponent(event){
      event.preventDefault();
      event.stopPropagation();
      let dropData = event.dataTransfer.getData('text/html');
      let dropItems = dropData.split("|");
      let draggedZone = document.getElementById(dropItems[0]);
      let droppedElement = document.getElementById(event.target.id);
      let draggedID = dropItems[1];
      
      // only try if the dragged element is not empty
      if(draggedID != 'empty' && document.getElementById(draggedID) != null){
          if(droppedElement != null && droppedElement.classList.contains('drop-area')){
            if(event.target.childNodes.length > 0){
              draggedZone.appendChild(event.target.childNodes[0]);
            }
            //console.log(draggedID);
            droppedElement.appendChild(document.getElementById(draggedID));
            droppedElement.classList.remove('drop-area-highlighted');
        } 
        else if(droppedElement){
          let element = droppedElement;
          while(element.parentNode){
            element = element.parentNode;
            if(element.classList.contains('drop-area')){
              //console.log(element.childNodes[0]);
              draggedZone.appendChild(element.childNodes[0]);
              element.appendChild(document.getElementById(draggedID));
              element.classList.remove('drop-area-highlighted');
              break;
            }
          }
        }
      } else{
        if(droppedElement != null){
          droppedElement.classList.remove('drop-area-highlighted');
        }
        
      }
      
      return false;
    },
    dragEnter(event){
    //console.log(event)
    if(event.target.classList.contains('drop-area')){
          event.target.classList.add('drop-area-highlighted');
        }
    },
    dragLeave(event){
      let element = document.getElementById(event.target.id);
      if(element != null){
        element.classList.remove('drop-area-highlighted');
      }
      
    },
    // loading and saving data for logging consent and uuid etc.
    hasStorage(){
        try {
            window.localStorage.setItem('test', 'storage');
            window.localStorage.removeItem('test');
            return true;
        } catch (exception) {
            return false;
          }
      },
      // UUID is generated by the practable booking system and stored in localStorage (as userName)
      // This function should just find that userName and set it in vuex, else set uuid to null
    updateUUID(){
        let stored_uuid;

        if(this.getUsesLocalStorage){
          stored_uuid = window.localStorage.getItem('userName');
        } else {
          stored_uuid = null;
        }
        
        if(stored_uuid){
            this.$store.dispatch('setUUID', stored_uuid);
        } else{
          this.$store.dispatch('setUUID', 'NA');
        }
      },
      
  },
}
</script>

<style>

</style>
