<template>
    <div id="settings">        
        <Loader v-if="loading"></Loader>
        <template v-else>
            <div class="setting-section">
                <h2>MOD Settings</h2>
                <div class="form-line">
                    <label>Active MOD</label>
                    <select v-model="activeMod" name="name">
                        <option v-for="mod in availableMods" :key="mod">{{mod}}</option>
                    </select>
                </div>
                <div class="form-line">
                    <label>Active Source</label>
                    <select v-model="activeSource" name="source">
                        <option value="0">Microphone</option>
                        <option value="1">Desktop</option>
                    </select>
                </div>
                <button v-on:click="setModSettings()" class="button">Save MOD Settings</button>
            </div>
            <div class="setting-section">
                <div class="flex-heading"><h2>WiFi Settings</h2><small>Connected to: {{connectedNetwork}}</small></div>
                <div class="form-line">
                    <label for="ssid">SSID: </label><input v-model="ssid" length=32 />
                </div>
                <div class="form-line">
                    <label for="pass">Pass: </label><input v-model="pass" length=64 />
                </div>
                <button v-on:click="setWifiSettings()" class="button">Save WiFi Settings</button>
            </div>
        </template>
    </div>
</template>

<script lang="ts">
    import { Component, Vue } from "vue-property-decorator";
    import Loader from "../components/Loader.vue";

    @Component({
        components: {
            Loader
        },
    })
    export default class Settings extends Vue {
        private loading: boolean = true;
        private activeMod : string = "";
        private activeSource : number = 0;
        private connectedNetwork: string = "NONE";
        private availableMods : Array<string> = new Array<string>();

        private ssid : string = "";
        private pass : string = "";

        async beforeMount() {
            try {
                await this.loadModList();
                await this.getModSettings();
                await this.getWifiSettings();
                this.loading = false;
            }
            catch(ex) {
                alert(ex);
            }            
        }

        async loadModList() : Promise<void> {            
            let response = await fetch("/api/mods/list", {
                method: "GET"
            });
            if(response.ok) {
                this.availableMods = (await response.json()).mods;
            }
            else { throw("Error during MOD list request!"); }
        }

        async getModSettings() : Promise<void> {            
            let response = await fetch("/api/mods/getModSettings", {
                method: "GET"
            });
            if(response.ok) {
                let modSettings = (await response.json()).modSettings;
                this.activeMod = modSettings.name;
                this.activeSource = modSettings.source;
            }
            else { throw("Error during MOD settings request!"); }
        }

        async setModSettings() : Promise<void> {
            this.loading = true;
            let response = await fetch(`/api/mods/setModSettings?name=${this.activeMod}&source=${this.activeSource}`, {
                method: "GET"
            });
            if(response.ok && (await response.json()).saved) {
                this.loading = false;
            }
            else { alert("Error saving MOD settings!"); }
        }

        async getWifiSettings() : Promise<void> {
            let response = await fetch("/api/wifi/get", {
                method: "GET"
            });
            if(response.ok) {
                let wifiInfo = (await response.json()).wifi;
                this.connectedNetwork = wifiInfo.connected ? wifiInfo.ssid : "NONE";
            }
            else { throw("Error during WiFi info request!"); }
        }

        async setWifiSettings() : Promise<void> {
            this.loading = true;
            let response = await fetch(`/api/wifi/set?ssid=${this.ssid}&pass=${this.pass}`, {
                method: "GET"
            });
            if(response.ok && (await response.json()).saved) {
                this.loading = false;
            }
            else { alert("Error saving WiFi settings!"); }
        }
    }
</script>

<style lang="scss">
    @import "./assets/sass/shared/_var.scss";

    #wifi {
        margin: 1rem 2rem;
    }

    #settings {
        display: flex;
        flex-wrap: wrap;
        min-width: 350px;
        margin: 0 -2rem 0 0;
    }

    .setting-section {
        flex: 1;
        margin: 0 2rem 2rem 0;

        h2 { 
            margin: 0 0 2rem 0;
        }
    }

    .form-line {
        margin-bottom: 0.75rem;
        display: grid;
        grid-template-areas: "left right";
        grid-template-columns: 100px 1fr;

        label { text-align: right; }
        input, select { margin-left: 0.75rem; padding: 5px; color: $text-color; }
    }

    .button { 
        width: 100%;
        padding: 10px;
        border-radius: 5px;
        box-sizing: border-box;
        color: white;
        border: none; 
        background: $secondary-color;
        cursor: pointer;

        &:hover {
            color: black;
            background: $primary-color;
        }
    }

    .flex-heading {
        display: flex;
        flex-wrap: wrap;
        align-items: flex-end;
        margin: 0 0 2rem 0;

        h2 { margin: 0 1rem 0 0; }
        small { 
            font-style: italic; 
            font-size: 1rem;
            font-weight: 100;
        }
    }
</style>