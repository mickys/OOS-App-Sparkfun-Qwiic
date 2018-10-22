<template>

  <div class="VL53L1XApp">
    <div class="distance-label h1"> {{distance}} mm
    </div>
    <div class="bar distance-bar">
      <div class="bar-item" role="progressbar" :style="{width: `${distancePercent}%`}">
      </div>
    </div>
  </div>

</template>

<script>
export default {
  data () {
    return {
      distance: 0,
      maxDistance: 2000
    }
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
