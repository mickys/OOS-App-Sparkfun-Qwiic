<template>
  <div id="app">

    <div class="control">

      <label class="form-switch float-right">
        <input type="checkbox" v-model="isAutoRefresh">
        <i class="form-icon"></i>Auto Refresh
      </label>
      <button class="refreshBtn btn float-right" v-if="!isAutoRefresh">Refresh</button>

    </div>


    <div class="divider" data-content="Connected QWIIC Devices"></div>

    <div class="productList">


      <div class="card productItem" v-for="product in productList">
        <div class="card-image productPhoto">
          <img src="http://via.placeholder.com/300" class="img-responsive">
        </div>
        <div class="card-header">
          <div class="card-title h5">{{product.name}}</div>
          <div class="card-subtitle text-gray">I2C Addr (0x{{product.deviceAddr}})</div>
        </div>
        <div class="card-footer">
          <a :href="product.productUrl" class="btn btn-error" target="_blank">Learn More</a>
        </div>
      </div>

    </div>

  </div>
</template>

<script>

import 'spectre.css/dist/spectre.min.css'
import 'spectre.css/dist/spectre-icons.min.css'
import OnionCDK from '@/OnionCDK.js'

export default {
  name: 'App',
  data () {
    return {
      isLoading: false,
      isAutoRefresh: false,
      productList: [
        {
          deviceAddr: '30',
          name: 'Example Product asdfasdf adsf asdfa df',
          imageUrl: '/relative/path/img',
          productUrl: 'http://via.placeholder.com/300'
        },
        {
          deviceAddr: '33',
          name: 'Example Product 2',
          imageUrl: '/relative/path/img',
          productUrl: 'http://via.placeholder.com/300'
        },
        {
          deviceAddr: '51',
          name: 'Example Product 3',
          imageUrl: '/relative/path/img',
          productUrl: 'http://via.placeholder.com/300'
        },
        {
          deviceAddr: '53',
          name: 'Example Product 4',
          imageUrl: '/relative/path/img',
          productUrl: 'http://via.placeholder.com/300'
        },
        {
          deviceAddr: '5f',
          name: 'Example Product 5',
          imageUrl: '/relative/path/img',
          productUrl: 'http://via.placeholder.com/300'
        }
      ]
    }
  },
  components: {
    OnionCDK
  },
  mounted () {
    OnionCDK.onCmd = function (command, result) {
      this.isLoading = false
      console.log(command, result)
    }.bind(this)
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
  margin-top: 20px;
  display: flex;
  align-items: flex-start;
  flex-direction: row;
  flex-wrap: wrap;
}

.productPhoto img {
  width: 375px;
  height: 375px;
}


.refreshBtn {
  margin-right: 20px;
}

.productItem {
  margin: 10px;
}

</style>
