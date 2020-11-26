<template>
    <div id="app" class="grid-container">
        <header class="grid-header">
            <div class="content">
                <div class="stretch vertical-center">
                    <a id="header-logo" href="/" title="Mods"><img alt="syncBlink Logo" src="./../assets/logo.png"></a>
                </div>
                <nav id="nav">
                    <router-link class="menu-item" to="/">Mods</router-link>
                    <router-link class="menu-item" to="/settings">Settings</router-link>
                </nav>
            </div>
        </header>
        <main class="grid-main">
            <router-view></router-view>
        </main>
        <footer class="grid-footer">
            <small>syncBlink - Created by Gerrit "Geaz" Gazic</small>
        </footer>
    </div>
</template>

<style lang="scss">
    @import "./assets/sass/application.scss";

    html, body {
        height: 100%;
        max-height: 100%;
    }

    body {
        color: $text-color;
        background: $background-color;
    }

    header {
        background: white;
        border-bottom: 1px solid $border-color;
        box-shadow: $box-shadow-bottom;

        #header-logo {
            display: inline-block;
            img { width: 100px; vertical-align: middle; }
        }

        a { 
            color: $primary-color;
            text-decoration: none;
        }

        .stretch {
            flex: 1;
        }
        
        .vertical-center {
            display: flex;
            align-items: center;
        }

        .content {
            display: flex;
            padding: 1rem 2rem;
        }

        .menu-item {
            margin-left: 25px;
            font-weight: bold;
            font-size: 1.1rem;
            text-decoration: none;
            color: #a1a0a0;
        }

        .router-link-exact-active {
            color: $text-color;
        }
    }

    main {
        padding: 1.5rem 2rem 0 2rem;
    }

    footer {
        font-size: 0.8rem;
        text-align: right;
        padding: 0.5rem 2rem;
    }

    #app {
        height: 100%;
        width: 100%;
    }

    a {
        color: $text-color;

        &:hover {
            color: $primary-color;
        }

        i {
            vertical-align: middle;
            color: $text-color-faded;
            &:hover { color: $primary-color; }
        }
    }

    span i { vertical-align: middle; }

    .grid-header { grid-area: header; }
    .grid-main { grid-area: main; }
    .grid-footer { grid-area: footer; }

    .grid-container {
        display: grid;
        grid-template-rows: auto 1fr auto;
        grid-template-areas:
            "header"
            "main"
            "footer";
    }
</style>