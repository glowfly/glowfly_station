<template>
    <div id="mods" class="grid-container">
        <Loader v-if="loading"></Loader>
        <template v-else>
            <aside class="grid-aside">
                <div id="avail-mods">
                    <label>Available Mods</label>
                    <a href="#" v-on:click="addModVisible = !addModVisible"><i class="material-icons-outlined">add_box</i></a>
                </div>
                <ul>
                    <li :class="{ 'active': activeMod === mod }" v-for="mod in availableMods" :key="mod">
                        <a href="#" class="main-link" v-on:click="loadMod(mod)">{{mod}}</a>

                        <span v-if="activeMod === mod && modContent === originalModContent" class="deactivated-tool"><i class="material-icons-outlined">save</i></span>
                        <a v-else-if="activeMod === mod && modContent !== originalModContent" v-on:click="saveMod()" href="#" class="tool"><i class="material-icons-outlined blue">save</i></a>

                        <a href="#" v-on:click="deleteMod(mod)" :class="{ 'tool': true, 'hover-hidden': true, 'visible': activeMod === mod }"><i class="material-icons-outlined hover-red">delete</i></a>
                    </li>
                </ul>
                <div id="add-mod" v-if="addModVisible">
                    <input v-focus v-on:keyup.enter="addMod()" v-model="newModName" type="text" placeholder="Enter MOD name"/>
                </div> 
            </aside>
            <CodeEditor class="grid-editor" v-model="modContent" />
        </template>
    </div>
</template>

<script lang="ts">
    import { Component, Vue, Watch } from "vue-property-decorator";
    import Loader from "../components/Loader.vue";
    import CodeEditor from "../components/CodeEditor.vue";

    @Component({
        components: {
            Loader,
            CodeEditor
        },
    })
    export default class Mods extends Vue {
        private loading : boolean = true;
        private availableMods : Array<string> = new Array<string>();
        private activeMod : string = "";
        private modContent : string = "";
        private originalModContent : string = "";
        private newModName : string = "";
        private addModVisible : boolean = false;

        async beforeMount() : Promise<void> {
            await this.loadModList();
            this.loading = false;
        }

        async loadModList() : Promise<void> {            
            let response = await fetch("/api/mods/list", {
                method: "GET"
            });
            if(response.ok) {
                this.availableMods = (await response.json()).mods;
                if(this.availableMods.length > 0 && this.activeMod === "") await this.loadMod(this.availableMods[0]);
            }
        }

        async loadMod(modName : string) : Promise<void> {
            if(this.modContent !== this.originalModContent) {
                if(confirm(`Unsaved changes available in "${this.activeMod}"! Do you want to save them?`)) {
                    await this.saveMod();
                }
            }

            this.activeMod = modName;
            let response = await fetch(`/api/mods/get?name=${this.activeMod}`, {
                method: "GET"
            });
            if(response.ok) {
                let modInfo = (await response.json()).mod;
                this.modContent = modInfo.content;
                this.originalModContent = modInfo.content;
            }
        }

        async addMod() : Promise<void> {
            this.loading = true;
            if(this.newModName != "") {
                let response = await fetch(`/api/mods/add?name=${this.newModName}`, {
                    method: "GET"
                });
                if(response.ok) {
                    await this.loadMod(this.newModName);
                    await this.loadModList();
                    
                    this.newModName = "";
                    this.addModVisible = false;
                    this.loading = false;
                }
                else {
                    alert("Error during MOD creation!");
                }                
            }
        }

        async saveMod() : Promise<void> {
            let mod = {
                name: this.activeMod,
                content: this.modContent
            };
            let response = await fetch(`/api/mods/save`, {
                method: "POST",
                body: JSON.stringify(mod)
            });
            if(response.ok) {
                this.originalModContent = this.modContent;
            }
            else {
                alert("Error during MOD save!");
            }
        }

        async deleteMod(modName : string) : Promise<void> {
            if(confirm(`Do you really want to delete the "${modName}" MOD?`)) {
                this.loading = true;
                let response = await fetch(`/api/mods/delete?name=${modName}`, {
                    method: "GET"
                });
                if(response.ok) {
                    this.activeMod = this.activeMod === modName ? "" : this.activeMod;
                    await this.loadModList();
                    this.loading = false;
                }
                else {
                    alert("Error during MOD deletion!");
                }
            }
        }
    }
</script>

<style scoped lang="scss">
    @import "./assets/sass/shared/_var.scss";

    .grid-aside { grid-area: aside; }
    .grid-editor { grid-area: editor; }

    .grid-container {
        display: grid;
        grid-template-rows: auto;
        grid-template-columns: auto 1fr;
        grid-template-areas:
            'aside editor';
    }

    #mods {
        width: 100%;
        height: 100%;
        max-height: 100%;
    }

    aside {
        width: 300px;
        margin-left: -2rem;
        box-sizing: border-box;

        ul {
            margin: 0;
            padding: 0;
            list-style: none;
        }

        li {
            display:flex;
            padding: 10px 1rem 10px 3rem;

            .main-link {
                flex-grow: 1;
                display: inline-block;
                box-sizing: border-box;
                text-decoration: none;
            }
        }

        li:hover {  
            .main-link { color: $primary-color; }          
            .hover-hidden { visibility: visible; }
        }

        .tool,
        .deactivated-tool {
            padding-right: 5px;
            i { color: lighten($text-color-faded, 20%); }
            i:hover { color: $primary-color; }

            .blue { color: $text-color-blue; }
            .hover-red:hover { color: $text-color-red; }
        }

        .deactivated-tool {
            i { color: lighten($text-color-faded, 30%); }
            i:hover { color: lighten($text-color-faded, 30%); }
        }

        .active { background: darken($background-color, 20%); }

        .hover-hidden { visibility: hidden; }
        .visible { visibility: visible; }
    }

    #avail-mods {
        display: flex;
        align-items: center;
        margin: 0 1rem 1rem 1.5rem;

        label { 
            flex-grow: 1;
            font-weight: lighter;
            font-size: 1.1rem;
            color: $text-color-faded;
        }

        a {
            padding: 5px;
        }
    }

    #add-mod {
        box-sizing: border-box;
        margin-top: 1rem;
        padding: 0 1.5rem;

        input { 
            padding: 5px; 
            width: 100%;
            box-sizing: border-box;
        }
    }

    #code-editor {
        max-height: 85vh;
        margin-right: -2rem;
        box-shadow: $box-shadow-outset;
    }
</style>