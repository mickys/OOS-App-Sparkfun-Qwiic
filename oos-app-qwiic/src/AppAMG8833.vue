<template>

  <div class="qwiicApp">

    <div class="range-input">
      <div style="display: flex; align-items: center; justify-content: center">
        Min:&nbsp;<input type="number" v-model="minT">
        <!-- &nbsp;&nbsp;&nbsp;&nbsp; -->
        Max:&nbsp;<input type="number" v-model="maxT"> &#8451;
        <!-- <input type="checkbox" v-model="monoChrome"> -->
      </div>
      <div class="" style="display: flex; align-items: center; justify-content: center">
        <label class="form-switch form-inline" >
          <input type="checkbox" v-model="monoChrome">
          <i class="form-icon"></i> Mono Chrome
        </label>

      </div>
      <!-- <label class="form-switch form-inline">
        <input type="checkbox" v-model="tempF">
        <i class="form-icon"></i> &#8451; | &#8457;
      </label> -->
    </div>

    <div class="scale">
      <div v-for="(pix, index) in colorScale" :style="pixelColor(pix)" class="scale-pixel" :key="`index-${index}`">
        <div class="scale-label" v-if="[0, 25, 50, 75, 99].indexOf(index) > -1">{{isNaN(pix)?'':pix}}</div>
      </div>
    </div>

    <div class="grid">
      <div
      v-for="(pix, index) in thermalArray"
      class="pixel tooltip"
      :data-tooltip="`${pix}`"
      :style="pixelColor(pix)"
      :key="`index-${index}`"

      ></div>
    </div>
  </div>

</template>

<script>
export default {
  data () {
    return {
      thermalArray: [],
      maxT: 30,
      minT: -5,
      tempF: false,
      monoChrome: false
    }
  },
  created () { },
  computed: {
    colorScale () {
      var tDiff = this.maxT - this.minT
      return Array.from(Array(100), (d, i) => (i * tDiff / 100 + parseFloat(this.minT)).toFixed(1))
    }
  },
  methods: {
    onMessage (topic, content) {
      this.$set(this, 'thermalArray', JSON.parse(content))
    },
    pixelColor (temp) {
      var value = (temp - this.minT) / (this.maxT - this.minT)
      var color = 'black'
      if (this.monoChrome) {
        color = `rgb(${value * 255}, ${value * 255}, ${value * 255})`
      } else {
        // Based on calculation from http://www.andrewnoske.com/wiki/Code_-_heatmaps_and_color_gradients
        var heatmap = [ [0, 0, 1], [0, 1, 0], [1, 1, 0], [1, 0, 0] ]
        var numColors = heatmap.length

        var idx1
        var idx2
        var fractBetween = 0

        if (value === 0) {
          idx1 = 0
          idx2 = 0
        } else if (value >= 1) {
          idx1 = numColors - 1
          idx2 = numColors - 1
        } else {
          value = value * (numColors - 1)
          idx1 = Math.floor(value)
          idx2 = idx1 + 1
          fractBetween = value - idx1
        }

        var red = (heatmap[idx2][0] - heatmap[idx1][0]) * fractBetween + heatmap[idx1][0]
        var green = (heatmap[idx2][1] - heatmap[idx1][1]) * fractBetween + heatmap[idx1][1]
        var blue = (heatmap[idx2][2] - heatmap[idx1][2]) * fractBetween + heatmap[idx1][2]

        color = `rgb(${red * 255}, ${green * 255}, ${blue * 255})`
      }
      return {'background-color': color}
    }
  }
}
</script>

<style>
.qwiicApp {
  width: 100%;
  /* background: gray; */
  display: flex;
  justify-content: center;
  align-items: center;
  flex-direction: column;
  height: 100%;
}
.scale {
  display: flex;
  flex-direction: row;
  height: 20px;
  width: 320px;
  margin-bottom: 40px;
}
.scale-pixel {
  width: 1%;
  height: 100%;
}
.scale-label {
  position: relative;
  bottom: -20px;
  left: -0.5em;
}
.grid {
  display: flex;
  flex-direction: column;
  flex-wrap: wrap;
  width: 320px;
  height: 320px;
}
.pixel {
  height: 40px;
  width: 40px;
  /* background-color: black; */
}
.range-input {
  width: 320px;
  margin-bottom: 10px;
}
.range-input input {
  width: 40px;
  margin-right: 10px;
}
</style>
