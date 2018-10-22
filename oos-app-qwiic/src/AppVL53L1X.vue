<template>

  <div class="VL53L1XApp">

    <div class="loading loading-lg" v-if="distance === '--'"></div>
    <div class="" v-if="distance === '--'"> Starting ... </div>

    <div class="distance-label h1" v-if="distance !== '--'"> {{distance}} mm
    </div>
    <div class="bar distance-bar" v-if="distance !== '--'">
      <div class="bar-item" role="progressbar" :style="{width: `${distancePercent}%`}">
      </div>
    </div>
  </div>

</template>

<script>
export default {
  data () {
    return {
      distance: '--',
      maxDistance: 2000
    }
  },
  created () {
    this.$emit('onCreated', 'oos-qwiic-vl53l1x')
  },
  destroyed () {
    this.$emit('onDestroyed', 'oos-qwiic-vl53l1x')
  },
  computed: {
    distancePercent () {
      return this.distance * 100 / this.maxDistance
    }
  },
  methods: {
    onMessage (topic, content) {
      if (topic === '/console/qwiic-vl53l1x') {
        this.$set(this, 'distance', JSON.parse(content))
      }
    }
  }
}
</script>

<style>
.VL53L1XApp {
  width: 100%;
  /* background: gray; */
  display: flex;
  justify-content: center;
  align-items: center;
  flex-direction: column;
  height: 100%;
}

.distance-label {
  color: #3498db;
}

.bar.distance-bar {
  width: 80%;
  height: 40px;
  border-radius: 40px;
  margin: 10px;
  margin-bottom: 80px;
  overflow: hidden;
}

.bar.distance-bar .bar-item {
  width: 80%;
  height: 40px;
  border-radius: 40px;
  background-color: #3498db;
}

</style>
