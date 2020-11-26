<template>
    <div id="settings">        
        <Loader v-if="loading"></Loader>
        <template v-else>
            <p>Connected to: {{connectedNetwork}}</p>
            <form method="get" action="setSSID">
                <label for="ssid">SSID: </label><input name="ssid" length=32>
                <label for="pass">Pass: </label><input name="pass" length=64>
                <input type="submit">
            </form>
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
        private connectedNetwork: string = "NONE";

        async beforeMount() {
            try {
                let response = await fetch("/api/wifi/get", {
                    method: "GET"
                });
                if(response.ok) {
                    let wifiInfo = (await response.json()).wifi;
                    this.connectedNetwork = wifiInfo.connected ? wifiInfo.ssid : "NONE";
                    this.loading = false;
                }
                else { throw("Error during WiFi info request!"); }
            }
            catch(ex) {
                alert(ex);
            }            
        }
    }
</script>

<style lang="scss">
    #wifi {
        margin: 1rem 2rem;
    }
</style>