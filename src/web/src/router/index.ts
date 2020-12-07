import Vue from "vue"
import VueRouter, { RouteConfig } from "vue-router"
import Mods from "../views/Mods.vue"
import Settings from "../views/Settings.vue"

Vue.use(VueRouter);

const routes: Array<RouteConfig> = [
  {
    path: "/",
    name: "Settings",
    component: Settings
  },
  {
    path: "/mods",
    name: "Mods",
    component: Mods
  }
];

const router = new VueRouter({
  mode: "hash",
  base: process.env.BASE_URL,
  routes
});

export default router;
