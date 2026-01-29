//Modified version of original Snapshot component.
//This also acts as the DataRecorder so snapshots are actually saved to data.

<template>
<div class="m-2 practable-component" id='record-snapshot-div'>

    <div class='d-grid gap-2 d-sm-block'>
        <button id="record-snapshot-button" type='button' class="button-xsm button-primary" @click="takeSnapshot">Record Snapshot</button>
        <button id="reset-snapshot-button" type='button' class="button-xsm button-warning" @click="toggleResetModal">Reset</button>
        <button id="download-snapshot-button" type='button' class="button-xsm button-secondary" @click="outputToCSV">Download</button>
        <!-- <button id="new-dataset-button" type='button' class="button-xsm button-secondary" @click="addToDatasetIndex">New Dataset</button> -->
    </div>

    <div class="row justify-content-center align-items-center" @mousedown="setDraggable(false)" @mouseup="setDraggable(true)">

        <table class="table" >
            <thead>
                <tr>
                    <th v-for='heading in headings' :key="heading" scope="col">{{heading}}</th>
                </tr>
            </thead>

            <tbody>
                <tr>
                    <td class='hidden'></td>
                    <td class='hidden'></td>

                    <td class='current'>
                        <input type="text" id="frequency-input" v-model="frequency" class="form-control" placeholder="Frequency" aria-label="frequency-input" aria-describedby="manual input of frequency">
                    </td>

                    <td class='current'>
                        <input type="text" id="wavelength-input" v-model="wavelength" class="form-control" placeholder="Wavelength" aria-label="wavelength-input" aria-describedby="manual input of wavelength">
                    </td>

                    <td class='current'>{{ speed.toFixed(2) }}</td>

                    <td class='hidden'></td>
                </tr>

                <tr v-for="row in getData" :id="row.time" :key="row.time">
                    <td v-for='key in Object.keys(row)' :key="key" :id='key'>{{row[key]}}</td>
                    <button type='button' class='button-toolbar button-danger' id='delete-data-row-button' aria-label='delete data row button' @click='deleteDataRow(row.id)'>
                        <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="currentColor" class="bi bi-trash3" viewBox="0 0 16 16">
                            <path d="M6.5 1h3a.5.5 0 0 1 .5.5v1H6v-1a.5.5 0 0 1 .5-.5M11 2.5v-1A1.5 1.5 0 0 0 9.5 0h-3A1.5 1.5 0 0 0 5 1.5v1H1.5a.5.5 0 0 0 0 1h.538l.853 10.66A2 2 0 0 0 4.885 16h6.23a2 2 0 0 0 1.994-1.84l.853-10.66h.538a.5.5 0 0 0 0-1zm1.958 1-.846 10.58a1 1 0 0 1-.997.92h-6.23a1 1 0 0 1-.997-.92L3.042 3.5zm-7.487 1a.5.5 0 0 1 .528.47l.5 8.5a.5.5 0 0 1-.998.06L5 5.03a.5.5 0 0 1 .47-.53Zm5.058 0a.5.5 0 0 1 .47.53l-.5 8.5a.5.5 0 1 1-.998-.06l.5-8.5a.5.5 0 0 1 .528-.47M8 4.5a.5.5 0 0 1 .5.5v8.5a.5.5 0 0 1-1 0V5a.5.5 0 0 1 .5-.5"/>
                        </svg>
                  </button>
                </tr>

                
            </tbody>
                            
        </table> 

	</div>


    
    
    <div class="d-flex flex-row">
        <popup-help id="popup-help-snapshot">
            <template v-slot:header>
                <h5> Snapshot tool </h5>
            </template>
            <template v-slot:body>
                <div class='row mb-2'>
                    <div class='col'>
                        <p> Click 'Record Snapshot' to save the current state to the snapshot table. Every time you click a new data set will be added. Click 'Download Snapshots'
                            to download all the snapshots as a .csv file.
                        </p>
                    </div>
                </div>

            </template>
        </popup-help>
    </div>
    

    <div v-if='showResetConfirmModal' class="modal modal-show" tabindex="-1" role="dialog">
        <div class="modal-dialog modal-dialog-centered modal-dialog-scrollable" role="document">
          <div class="modal-content">
            <div class="modal-header">
              <h5 class="modal-title">Reset Data</h5>
              <button type="button" class="btn btn-close" data-bs-dismiss="modal" aria-label="Close" @click='toggleResetModal'>
                
              </button>
            </div>
            <div class="modal-body">
              <p>Are you sure you want to Reset? This will clear all stored data.</p>
            </div>
            <div class="modal-footer">
              <button type="button" class="button-xsm button-danger" @click="resetSnaps(); toggleResetModal();">Reset</button>
              <button type="button" class="button-xsm button-secondary" data-dismiss="modal" data-bs-dismiss="modal" @click="toggleResetModal">Cancel</button>
            </div>
          </div>
        </div>
      </div>

</div>
</template>

<script>
import { mapGetters, mapActions } from 'vuex';
import PopupHelp from './elements/PopupHelp.vue';

export default {
    name: 'SnapshotRecorder',
    props:[
      'headings'
    ],
    components:{
        PopupHelp,
    },
    data(){
        return{
            //snaps: [],
            showResetConfirmModal: false,
            id_index: 0
        }
    },
    computed:{
        ...mapGetters([
            'getNumData',
            'getDatasetIndex',
            'getData',
            'getCurrentTime',
            'getSecondsSinceStart',
            'getIsRecording',
            'getDarkTheme',
            'getCurrentWavelength',
            'getCurrentFrequency'
            
        ]),
        wavelength: {
			get(){
				return this.getCurrentWavelength;
			},
			set(val){
				this.setCurrentWavelength(val);
				//console.log('updating wavelength' + this.getCurrentWavelength);
			}
		},
        frequency: {
			get(){
				return this.getCurrentFrequency;
			},
			set(val){
				this.setCurrentFrequency(val);
				//console.log('updating frequency' + this.getCurrentFrequency);
			}
		},
        speed() {
            return this.wavelength * this.frequency;
        }
    },
    methods: {
        ...mapActions([
            'addData',
            'clearAllData',
            'setDatasetIndex',
            'addToDatasetIndex',
            'clearAllData',
            'deleteData',
            'setDraggable',
            'setCurrentWavelength',
            'setCurrentFrequency'
        ]),
        deleteDataRow(id){
            //delete an entry in the data array based on the id of that entry. id may no longer be sequential but are unique, so need to search array 
            // for first example of that unique id and use deleteData with the corresponding index in the data array.
            const isMatchingID = (entry) => entry.id == id;
            let indexToDelete = this.getData.findIndex(isMatchingID)
            if(indexToDelete != -1){
                this.deleteData(indexToDelete);
            }
        },
        takeSnapshot(){
            let new_snap = {
                id: this.id_index,  
                set: this.getDatasetIndex, 
                frequency: this.getCurrentFrequency,
                wavelength: this.getCurrentWavelength,
                speed: this.speed.toFixed(2),
                period: (1/this.getCurrentFrequency).toFixed(2),
               
            };
            this.addData(new_snap);
            this.id_index += 1;
            
        },
        resetSnaps(){
            this.clearAllData();
            this.setDatasetIndex(0);
            this.id_index = 0;
        },
        toggleResetModal(){
            this.showResetConfirmModal = !this.showResetConfirmModal;
        },
        outputToCSV(){
        let current_dataset = 0;
        // let csv = 'Timestamp[s],SetSpeed[rpm],ReportedSpeed[rpm],Displacement[mm]\n';
        let csv = 'Frequency[Hz],Wavelength[cm],Speed[cm/s]\n';
        let date = new Date();

        this.getData.forEach(function(d){
            if(d.set == current_dataset + 1){
                let hiddenElement = document.createElement('a');
                hiddenElement.href = 'data:text/csv;charset=utf-8,' + encodeURI(csv);
                hiddenElement.target = '_blank';
                hiddenElement.download = `RippleTank-${date.getHours()}-${date.getMinutes()}-dataset${current_dataset}.csv`;
                hiddenElement.click();

                // csv = 'Timestamp[s],SetSpeed[rpm],ReportedSpeed[rpm],Displacement[mm]\n';
                csv = 'Frequency[Hz],Wavelength[cm],Speed[cm/s]\n';
                current_dataset += 1;
              }

              csv += d.frequency.toString();
              csv += ",";
              csv += d.wavelength.toString();
              csv += ",";
              csv += d.speed.toString();
              
              csv += "\n";
        });

        //output the final dataset
        let hiddenElement = document.createElement('a');
        hiddenElement.href = 'data:text/csv;charset=utf-8,' + encodeURI(csv);
        hiddenElement.target = '_blank';
        hiddenElement.download = `RippleTank-${date.getHours()}-${date.getMinutes()}-dataset${current_dataset}.csv`;
        hiddenElement.click();
    },
    },
      
}
</script>

<style scoped>

.current{
    border-top: 1px solid var(--background-color-inverted);
    border-bottom: 1px solid var(--background-color-inverted);
    background-color: lightgreen;
    color: black;
}

.h-divider{
 margin-top:5px;
 margin-bottom:5px;
 height:1px;
 width:100%;
 border-top:1px solid gray;
}

.v-divider{
 margin-left:5px;
 margin-right:5px;
 padding: 0px;
 width:1px;
 height:100%;
 border-right:1px solid gray;
}

.modal-show{
    display: block;
}

.hidden{
    display: none;
}

#id{
    display: none;
}

#set{
    display: none;
}

#period{
    display: none;
}
</style>
