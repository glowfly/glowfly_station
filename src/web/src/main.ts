import Vue from "vue";
import App from "./App.vue";
import router from "./router";

import "./directives/focus";

Vue.config.productionTip = false;

new Vue({
    router,
    data: { loading: false },
    render: h => h(App)
}).$mount("#app");