<template>

  <div class="qwiicEnvApp">
    <div class="" v-if="!started">
      <div class="loading loading-lg"></div>
      <div class=""> Starting ... </div>
    </div>

    <div class="reading" v-if="started" :style="{color: r.color}" v-for="(r, key) in readings" :key="key">
      <div class="reading-value"  >
        <span class="h1">{{r.value}}</span>
        <span class="h5" v-html="r.unit">{{r.unit}}</span>
      </div>
      <div class="reading-label h5" v-html="r.label"> {{r.label}} </div>
    </div>
  </div>

</template>

<script>
export default {
  data () {
    return {
      started: false,
      readings: {
        temperature: {
          value: 23.4,
          label: 'Temperature',
          unit: '&#8451;',
          color: '#e74c3c'
        },
        humidity: {
          value: 51.9,
          label: 'Humidity',
          unit: '%',
          color: '#3498db'
        },
        pressure: {
          value: 101.5,
          label: 'Pressure',
          unit: 'kPa',
          color: '#f39c12'
        },
        co2: {
          value: 650,
          label: 'CO<sup>2</sup>',
          unit: 'ppm',
          color: '#7f8c8d'
        },
        tvoc: {
          value: 125,
          label: 'TVOC',
          unit: 'ppb',
          color: '#1abc9c'
        }
      }
    }
  },
  created () {
    this.$emit('onCreated', 'oos-qwiic-env')
  },
  destroyed () {
    this.$emit('onDestroyed', 'oos-qwiic-env')
  },
  computed: {
  },
  methods: {
    onMessage (topic, content) {
      if (topic === '/console/qwiic-env') {
        this.$set(this.readings.temperature, 'value', JSON.parse(content).temperature.toFixed(2))
        this.$set(this.readings.humidity, 'value', JSON.parse(content).humidity.toFixed(2))
        this.$set(this.readings.pressure, 'value', JSON.parse(content).pressure.toFixed(1))
        this.$set(this.readings.co2, 'value', JSON.parse(content).co2.toFixed(1))
        this.$set(this.readings.tvoc, 'value', JSON.parse(content).tvoc.toFixed(1))
        this.started = true
      }
    }
  }
}
</script>

<style>
.qwiicEnvApp {
  display: flex;
  flex-direction: row;
  justify-content: center;
  flex-wrap: wrap;
}

.reading {
  border: solid 1px #ecf0f1;
  height: 180px;
  width: 180px;
  border-radius: 20px;
  box-shadow: 0 0.2rem 0.5rem rgba(69,77,93,.15);
  margin: 15px;

  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}

.modal-container {
  height: inherit;
}

</style>
