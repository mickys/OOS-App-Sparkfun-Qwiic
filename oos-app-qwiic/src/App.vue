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

    <div class="divider text-center" data-content="Connected QWIIC devices will show up below"></div>

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
          <a v-if="product.component" class="btn btn-success float-right mt-2" @click="interact(product)">Interact</a>
          <a :href="product.productUrl" class="btn btn-link float-right" target="_blank">Learn More</a>
        </div>
      </div>

    </div>

    <div class="modal active" v-if="interactProduct">
      <a class="modal-overlay" @click="interact(null)"></a>
      <div class="modal-container">
        <div class="modal-header">
          <a class="btn btn-clear float-right" @click="interact(null)"></a>
          <div class="product-modal-title">
            <img class="header-image" :src="interactProduct.imageUrl" alt="">
            <div class="modal-title">
              <div class="h5">{{interactProduct.name}}</div>
              <div class="text-gray">I2C Addr ({{interactProduct.deviceAddr}})</div>
            </div>
          </div>
        </div>
        <div class="modal-body">
          <div class="content">
            <component
            :is="interactProduct.component"
            @onCreated = "onCreated"
            @onDestroyed = "onDestroyed"
            ref="activeProduct"
            />
          </div>
        </div>

      </div>
    </div>

  </div>
</template>

<script>

import 'spectre.css/dist/spectre.min.css'
import 'spectre.css/dist/spectre-icons.min.css'
import ProductDB from '@/productDB.js'
import OnionCDK from '@/OnionCDK.js'
import AppAMG8833 from '@/AppAMG8833'
import AppEnvironmental from '@/AppEnvironmental'
import AppVL53L1X from '@/AppVL53L1X'

export default {
  name: 'App',
  components: {
    AppAMG8833,
    AppEnvironmental,
    AppVL53L1X
  },
  data () {
    return {
      isLoading: false,
      isAutoRefresh: true,
      interactProduct: null,
      productList: []
    }
  },
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

    OnionCDK.onInit = function () {
      OnionCDK.subscribe('/console/qwiic-amg8833')
      OnionCDK.subscribe('/console/qwiic-env')
      OnionCDK.subscribe('/console/qwiic-vl53l1x')
    }

    OnionCDK.onMessage = function (topic, content) {
      if (this.$refs.activeProduct) this.$refs.activeProduct.onMessage(topic, content)
    }.bind(this)

    OnionCDK.init()

    setInterval(function () {
      if (this.isAutoRefresh && !this.isLoading) {
        this.getData()
      }
    }.bind(this), 1000)
  },
  methods: {
    onCreated (service) {
      console.log('app created')
      console.log(service)
      OnionCDK.service(service, 'start')
    },
    onDestroyed (service) {
      console.log('app destroyed')
      console.log(service)
      OnionCDK.service(service, 'stop')
    },
    interact (product) {
      this.interactProduct = product
    },
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

.modal-container {
  border-radius: 20px;
  padding: 0;
  /* height: 600px; */
  max-width: 800px;
}

.modal-container .modal-body {
  height: 100%;
}

.product-modal-title {
  display: flex;
  flex-direction: row;
  align-items: center;
}

.product-modal-title img {
  height: 60px;
  border-radius: 50%;
}

.product-modal-title .modal-title {
  margin-left:10px;
}

</style>
