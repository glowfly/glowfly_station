<template>
  <div id="code-editor">
      <textarea id="code"></textarea>
  </div>
</template>

<script lang="ts">
    import "codemirror/lib/codemirror.css";
    import "codemirror/mode/javascript/javascript";

    import CodeMirror from "codemirror";
    import { Component, Prop, Vue, Watch } from "vue-property-decorator";

    @Component
    export default class CodeEditor extends Vue {
        @Prop() private value!: string;
        private codeEditor!: CodeMirror.EditorFromTextArea;

        mounted() {
            let codeEditorArea = <HTMLTextAreaElement>document.getElementById("code");
            if(codeEditorArea !== null) {
                this.codeEditor = CodeMirror.fromTextArea(codeEditorArea, {
                    lineNumbers: true,
                    lineWrapping: true,
                    mode: "javascript",
                    indentUnit: 4
                });
                this.codeEditor.setSize("100%", "100%");
                this.codeEditor.setValue(this.value);
                this.codeEditor.on("change", () => this.$emit("input", this.codeEditor.getValue()));
            } 
            else {
                console.error("Code Editor TextArea not found!");
            }
        }

        @Watch("value")
        onValueChanged(val: string, oldVal: string) {
            let pos = this.codeEditor.getCursor();
            this.codeEditor.setValue(this.value);
            this.codeEditor.setCursor(pos);
        }
    }
</script>

<style lang="scss">
    #code-editor {
        background: white;
        border-radius: 10px 0 0 10px;
    }

    .CodeMirror {        
        border-radius: 10px 0 0 10px;
    }
</style>
