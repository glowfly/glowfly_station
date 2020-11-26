module.exports = {
    productionSourceMap: false,
    outputDir: "../firmware/data/public",
    chainWebpack: config => {
        config
            .plugin('html')
            .tap(args => {
                args[0].title = 'syncBlink'
                return args
            })
    }
}