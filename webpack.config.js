const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');
const outputDir = path.join(__dirname, 'build/');

const isProd = process.env.NODE_ENV === 'production';

module.exports = {
  entry: './src/Index.bs.js',
  mode: isProd ? 'production' : 'development',
  output: {
    path: outputDir,
    filename: 'bundle.js'
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: 'index.html',
      inject: false
    })
  ],
  module: {
    rules: [
      {
        test: /\.css$/i,
        use: ['css-loader'],
      },
    ]
  },
  externals: {
    "fs": "commonjs fs"
  },
  devServer: {
    compress: true,
    contentBase: outputDir,
    port: process.env.PORT || 8080,
    historyApiFallback: true
  },
  experiments: {
    topLevelAwait: true
  },
  resolve: {
    alias: { "path": "path-browserify" }
  }
};