<template>
  <div id="app">

    <div class="control">

      <label class="form-switch float-right">
        <input type="checkbox" v-model="isAutoRefresh"
        :true-value="true"
        :false-value="false">
        <i class="form-icon"></i>Auto Refresh
      </label>
      <button class="refreshBtn btn float-right" v-if="!isAutoRefresh"
      @click="onRefresh">Refresh</button>

    </div>

    <div class="divider" data-content="Connected QWIIC devices will show up below"></div>

    <div class="productList">

      <div class="card productItem"  v-for="(product, index) in productList" :key="index">
        <div class="card-image productPhoto">
          <img :src="product.imageUrl" class="img-responsive">
        </div>
        <div class="productName">
          <div class="card-title h5">{{product.name}}</div>
          <div class="card-subtitle text-gray">I2C Addr ({{product.deviceAddr}})</div>
        </div>
        <div class="productFooter">
          <a :href="product.productUrl" class="btn btn-error float-right" target="_blank">Learn More</a>
          <a class="btn float-right disabled mt-2 tooltip tooltip-right" data-tooltip="Coming Soon">Interact</a>
        </div>
      </div>

    </div>

  </div>
</template>

<script>

import 'spectre.css/dist/spectre.min.css'
import 'spectre.css/dist/spectre-icons.min.css'
import ProductDB from '@/productDB.json'
import OnionCDK from '@/OnionCDK.js'

export default {
  name: 'App',
  data () {
    return {
      isLoading: false,
      isAutoRefresh: true,
      productList: []
    }
  },
  // components: {
  //   OnionCDK
  // },
  mounted () {
    OnionCDK.onCmd = function (command, result) {
      this.isLoading = false
      if (command === 'i2cdetect') {
        var data = result
        this.productList = []
        var deviceList = this.parseData(data)
        for (var i = 0; i < deviceList.length; i++) {
          var deviceAddr = deviceList[i]
          var device = this.lookUpProduct('0x' + deviceAddr.toUpperCase())
          if (device) this.productList.push(device)
        }
      }
    }.bind(this)

    OnionCDK.init()

    setInterval(function () {
      if (this.isAutoRefresh && !this.isLoading) {
        this.getData()
      }
    }.bind(this), 1000)
  },
  methods: {
    onRefresh () {
      this.getData()
    },
    getData () {
      this.isLoading = true
      OnionCDK.sendCmd('i2cdetect', ['-y', '0'])
    },
    parseData (data) {
      var result = []
      for (var line of data.split('\n')) {
        line = line.split(':')
        if (line.length > 1) {
          for (var addr of line[1].split(' ')) {
            if (['UU', '--', ' ', '', '-', '—'].indexOf(addr) > -1) continue
            result.push(addr)
          }
        }
      }
      return result
    },
    lookUpProduct (deviceAddr) {
      // console.log(deviceAddr)
      for (var i = 0; i < ProductDB.length; i++) {
        // console.log(`${ProductDB[i].deviceAddr} === ${deviceAddr}`)
        if (ProductDB[i].deviceAddr === deviceAddr) {
          return ProductDB[i]
        }
      }
      return null
    }
  }
}
</script>

<style>

body {
  display: flex;
  flex-direction: row;
  justify-content: center;
}
#app {
  width: 100%;
  max-width: 800px;
  display: flex;
  flex-direction: column;
  justify-content: center;
  /* background: #eee; */
}

.control {
  margin-top: 20px;
}

.productList {
  margin-top: 10px;
  display: flex;
  /* align-items: flex-start; */
  flex-direction: column;
  /* flex-wrap: wrap; */
}

.productPhoto img {
  width: auto;
  height: 150px;
}

.refreshBtn {
  margin-right: 20px;
}

.card {
  margin: 5px;
  flex-direction: row!important;
}

.productName {
  margin: auto;
}

.productFooter {
  margin: auto;
  margin-right: 20px;
  display: inline-grid;
}

.onlineBadge {
  width: 20px;
  height: 20px;
  background-color: red
}

</style>
