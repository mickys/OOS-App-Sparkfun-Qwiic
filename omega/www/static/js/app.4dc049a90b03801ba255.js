webpackJsonp([1],{"5Foo":function(t,e){},NHnr:function(t,e,n){"use strict";Object.defineProperty(e,"__esModule",{value:!0});var r=n("7+uW"),i=n("BO1k"),a=n.n(i),s=(n("i8c9"),n("Wq+4"),[{name:"Distance Sensor Breakout - 4 Meter, VL53L1X",deviceAddr:"0x29",productUrl:"https://www.sparkfun.com/products/14722",imageUrl:"static/14722.jpg",component:"AppVL53L1X"},{name:"Real Time Clock Module - RV",deviceAddr:"0xD2",productUrl:"https://www.sparkfun.com/products/14558",imageUrl:"static/14558.jpg"},{name:"Grid Infrared Array Breakout - AMG8833",deviceAddr:"0x68",productUrl:"https://www.sparkfun.com/products/14607",imageUrl:"static/14607.jpg",component:"AppAMG8833"},{name:"Grid Infrared Array Breakout - AMG8833",deviceAddr:"0x69",productUrl:"https://www.sparkfun.com/products/14607",imageUrl:"static/14607.jpg",component:"AppAMG8833"},{name:"Triple Axis Magnetometer Breakout - MLX90393",deviceAddr:"0xC0",productUrl:"https://www.sparkfun.com/products/14571",imageUrl:"static/14571.jpg"},{name:"VR IMU Breakout - BNO080",deviceAddr:"0x4A",productUrl:"https://www.sparkfun.com/products/14686",imageUrl:"static/14686.jpg"},{name:"VR IMU Breakout - BNO080",deviceAddr:"0x4B",productUrl:"https://www.sparkfun.com/products/14686",imageUrl:"static/14686.jpg"},{name:"Distance Sensor Breakout - RFD77402",deviceAddr:"0x4C",productUrl:"https://www.sparkfun.com/products/14539",imageUrl:"static/14539.jpg"},{name:"Triple Axis Accelerometer Breakout - MMA8452Q",deviceAddr:"0x1D",productUrl:"https://www.sparkfun.com/products/14587",imageUrl:"static/14587.jpg"},{name:"Triple Axis Accelerometer Breakout - MMA8452Q",deviceAddr:"0x1C",productUrl:"https://www.sparkfun.com/products/14587",imageUrl:"static/14587.jpg"},{name:"Environmental Combo Breakout - CCS811",deviceAddr:"0x5A",productUrl:"https://www.sparkfun.com/products/14348",imageUrl:"static/14348.jpg",component:"AppEnvironmental"},{name:"Environmental Combo Breakout - CCS811",deviceAddr:"0x5B",productUrl:"https://www.sparkfun.com/products/14348",imageUrl:"static/14348.jpg",component:"AppEnvironmental"},{name:"Environmental Combo Breakout - BME280",deviceAddr:"0x76",productUrl:"https://www.sparkfun.com/products/14348",imageUrl:"static/14348.jpg"},{name:"Environmental Combo Breakout - BME280",deviceAddr:"0x77",productUrl:"https://www.sparkfun.com/products/14348",imageUrl:"static/14348.jpg",component:"AppEnvironmental"},{name:"Spectral Sensor Breakout - AS7263 NIR",deviceAddr:"0x49",productUrl:"https://www.sparkfun.com/products/14351",imageUrl:"static/14351.jpg"},{name:"GPS Breakout - XA1110",deviceAddr:"0x10",productUrl:"https://www.sparkfun.com/products/14414",imageUrl:"static/14414.jpg"},{name:"Micro OLED Breakout",deviceAddr:"0X3D",productUrl:"https://www.sparkfun.com/products/14532",imageUrl:"static/14532.jpg"},{name:"Micro OLED Breakout",deviceAddr:"0X3C",productUrl:"https://www.sparkfun.com/products/14532",imageUrl:"static/14532.jpg"},{name:"Human Presence Sensor Breakout - AK9753",deviceAddr:"0x64",productUrl:"https://www.sparkfun.com/products/14349",imageUrl:"static/14349.jpg"},{name:"Human Presence Sensor Breakout - AK9753",deviceAddr:"0x65",productUrl:"https://www.sparkfun.com/products/14349",imageUrl:"static/14349.jpg"},{name:"Human Presence Sensor Breakout - AK9753",deviceAddr:"0x67",productUrl:"https://www.sparkfun.com/products/14349",imageUrl:"static/14349.jpg"}]);function o(){return Math.random().toString(36).substring(2)}var c={name:"ConsoleSDK",appUid:"",makeId:o,init:function(){window.addEventListener("message",this.processMessage.bind(this),!1)},subscribe:function(t){this.sendEvent("Onion.CDK.Subscribe",{topic:t})},publish:function(t,e){},service:function(t,e,n){this.sendEvent("Onion.CDK.Service",{service:t,command:e})},sendEvent:function(t,e){var n=o();return window.parent.postMessage({event:t,instance:this.appUid,eventId:n,content:e},"*"),n},sendCmd:function(t,e){this.sendEvent("Onion.CDK.Command",{cmd:t,params:e})},sendToast:function(t){this.sendEvent("Onion.CDK.Toast",{message:t})},processMessage:function(t){if("Onion.CDK.Init"===t.data.event){var e=t.data.appUid;this.appUid=e,this.onInit()}else"Onion.CDK.Service"===t.data.event?this.onService(t.data.content.name,t.data.content.command,t.data.content.result):"Onion.CDK.Message"===t.data.event?this.onMessage(t.data.content.topic,t.data.content.content):"Onion.CDK.Command"===t.data.event&&this.onCmd(t.data.content.cmd,t.data.content.resp)},onInit:function(){},onService:function(t,e,n){},onMessage:function(){},onCmd:function(){}},d=n("c/Tr"),l=n.n(d),u={data:function(){return{thermalArray:[],maxT:30,minT:-5,tempF:!1,monoChrome:!1}},created:function(){this.$emit("onCreated","oos-qwiic-amg8833")},destroyed:function(){this.$emit("onDestroyed","oos-qwiic-amg8833")},computed:{colorScale:function(){var t=this,e=this.maxT-this.minT;return l()(Array(100),function(n,r){return(r*e/100+parseFloat(t.minT)).toFixed(1)})}},methods:{onMessage:function(t,e){"/console/qwiic-amg8833"===t&&this.$set(this,"thermalArray",JSON.parse(e))},pixelColor:function(t){var e=(t-this.minT)/(this.maxT-this.minT),n="black";if(this.monoChrome)n="rgb("+255*e+", "+255*e+", "+255*e+")";else{var r,i,a=[[0,0,1],[0,1,0],[1,1,0],[1,0,0]],s=a.length,o=0;0===e?(r=0,i=0):e>=1?(r=s-1,i=s-1):(e*=s-1,i=(r=Math.floor(e))+1,o=e-r),n="rgb("+255*((a[i][0]-a[r][0])*o+a[r][0])+", "+255*((a[i][1]-a[r][1])*o+a[r][1])+", "+255*((a[i][2]-a[r][2])*o+a[r][2])+")"}return{"background-color":n}}}},p={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{staticClass:"qwiicApp"},[n("div",{staticClass:"range-input"},[n("div",{staticStyle:{display:"flex","align-items":"center","justify-content":"center"}},[t._v("\n      Min: "),n("input",{directives:[{name:"model",rawName:"v-model",value:t.minT,expression:"minT"}],attrs:{type:"number"},domProps:{value:t.minT},on:{input:function(e){e.target.composing||(t.minT=e.target.value)}}}),t._v(" "),t._v("\n      Max: "),n("input",{directives:[{name:"model",rawName:"v-model",value:t.maxT,expression:"maxT"}],attrs:{type:"number"},domProps:{value:t.maxT},on:{input:function(e){e.target.composing||(t.maxT=e.target.value)}}}),t._v(" ℃\n      ")]),t._v(" "),n("div",{staticStyle:{display:"flex","align-items":"center","justify-content":"center"}},[n("label",{staticClass:"form-switch form-inline"},[n("input",{directives:[{name:"model",rawName:"v-model",value:t.monoChrome,expression:"monoChrome"}],attrs:{type:"checkbox"},domProps:{checked:Array.isArray(t.monoChrome)?t._i(t.monoChrome,null)>-1:t.monoChrome},on:{change:function(e){var n=t.monoChrome,r=e.target,i=!!r.checked;if(Array.isArray(n)){var a=t._i(n,null);r.checked?a<0&&(t.monoChrome=n.concat([null])):a>-1&&(t.monoChrome=n.slice(0,a).concat(n.slice(a+1)))}else t.monoChrome=i}}}),t._v(" "),n("i",{staticClass:"form-icon"}),t._v(" Mono Chrome\n      ")])])]),t._v(" "),n("div",{staticClass:"scale"},t._l(t.colorScale,function(e,r){return n("div",{key:"index-"+r,staticClass:"scale-pixel",style:t.pixelColor(e)},[[0,25,50,75,99].indexOf(r)>-1?n("div",{staticClass:"scale-label"},[t._v(t._s(isNaN(e)?"":e))]):t._e()])}),0),t._v(" "),n("div",{directives:[{name:"show",rawName:"v-show",value:0===t.thermalArray.length,expression:"thermalArray.length === 0"}],staticClass:"loading loading-lg"}),t._v(" "),n("div",{directives:[{name:"show",rawName:"v-show",value:0===t.thermalArray.length,expression:"thermalArray.length === 0"}]},[t._v(" Starting ... ")]),t._v(" "),n("div",{directives:[{name:"show",rawName:"v-show",value:t.thermalArray.length>0,expression:"thermalArray.length > 0"}],staticClass:"grid"},t._l(t.thermalArray,function(e,r){return n("div",{key:"index-"+r,staticClass:"pixel tooltip",style:t.pixelColor(e),attrs:{"data-tooltip":""+e}})}),0)])},staticRenderFns:[]};var m={data:function(){return{started:!1,readings:{temperature:{value:23.4,label:"Temperature",unit:"&#8451;",color:"#e74c3c"},humidity:{value:51.9,label:"Humidity",unit:"%",color:"#3498db"},pressure:{value:101.5,label:"Pressure",unit:"kPa",color:"#f39c12"},co2:{value:650,label:"CO<sup>2</sup>",unit:"ppm",color:"#7f8c8d"},tvoc:{value:125,label:"TVOC",unit:"ppb",color:"#1abc9c"}}}},created:function(){this.$emit("onCreated","oos-qwiic-env")},destroyed:function(){this.$emit("onDestroyed","oos-qwiic-env")},computed:{},methods:{onMessage:function(t,e){"/console/qwiic-env"===t&&(this.$set(this.readings.temperature,"value",JSON.parse(e).temperature.toFixed(2)),this.$set(this.readings.humidity,"value",JSON.parse(e).humidity.toFixed(2)),this.$set(this.readings.pressure,"value",JSON.parse(e).pressure.toFixed(1)),this.$set(this.readings.co2,"value",JSON.parse(e).co2.toFixed(1)),this.$set(this.readings.tvoc,"value",JSON.parse(e).tvoc.toFixed(1)),this.started=!0)}}},v={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{staticClass:"qwiicEnvApp"},[t.started?t._e():n("div",{},[n("div",{staticClass:"loading loading-lg"}),t._v(" "),n("div",{},[t._v(" Starting ... ")])]),t._v(" "),t._l(t.readings,function(e,r){return t.started?n("div",{key:r,staticClass:"reading",style:{color:e.color}},[n("div",{staticClass:"reading-value"},[n("span",{staticClass:"h1"},[t._v(t._s(e.value))]),t._v(" "),n("span",{staticClass:"h5",domProps:{innerHTML:t._s(e.unit)}},[t._v(t._s(e.unit))])]),t._v(" "),n("div",{staticClass:"reading-label h5",domProps:{innerHTML:t._s(e.label)}},[t._v(" "+t._s(e.label)+" ")])]):t._e()})],2)},staticRenderFns:[]};var h={data:function(){return{distance:"--",maxDistance:2e3}},created:function(){this.$emit("onCreated","oos-qwiic-vl53l1x")},destroyed:function(){this.$emit("onDestroyed","oos-qwiic-vl53l1x")},computed:{distancePercent:function(){return 100*this.distance/this.maxDistance}},methods:{onMessage:function(t,e){"/console/qwiic-vl53l1x"===t&&this.$set(this,"distance",JSON.parse(e))}}},f={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{staticClass:"VL53L1XApp"},["--"===t.distance?n("div",{staticClass:"loading loading-lg"}):t._e(),t._v(" "),"--"===t.distance?n("div",{},[t._v(" Starting ... ")]):t._e(),t._v(" "),"--"!==t.distance?n("div",{staticClass:"distance-label h1"},[t._v(" "+t._s(t.distance)+" mm\n  ")]):t._e(),t._v(" "),"--"!==t.distance?n("div",{staticClass:"bar distance-bar"},[n("div",{staticClass:"bar-item",style:{width:t.distancePercent+"%"},attrs:{role:"progressbar"}})]):t._e()])},staticRenderFns:[]};var g={name:"App",components:{AppAMG8833:n("VU/8")(u,p,!1,function(t){n("Qyra")},null,null).exports,AppEnvironmental:n("VU/8")(m,v,!1,function(t){n("pSRK")},null,null).exports,AppVL53L1X:n("VU/8")(h,f,!1,function(t){n("X8mX")},null,null).exports},data:function(){return{isLoading:!1,isAutoRefresh:!0,interactProduct:null,productList:[]}},mounted:function(){c.onCmd=function(t,e){if(this.isLoading=!1,"i2cdetect"===t){var n=e;this.productList=[];for(var r=this.parseData(n),i=0;i<r.length;i++){var a=r[i],s=this.lookUpProduct("0x"+a.toUpperCase());s&&this.productList.push(s)}}}.bind(this),c.onInit=function(){c.subscribe("/console/qwiic-amg8833"),c.subscribe("/console/qwiic-env"),c.subscribe("/console/qwiic-vl53l1x")},c.onMessage=function(t,e){this.$refs.activeProduct&&this.$refs.activeProduct.onMessage(t,e)}.bind(this),c.init(),setInterval(function(){this.isAutoRefresh&&!this.isLoading&&this.getData()}.bind(this),1e3)},methods:{onCreated:function(t){c.service(t,"start")},onDestroyed:function(t){c.service(t,"stop")},interact:function(t){this.interactProduct=t},onRefresh:function(){this.getData()},getData:function(){this.isLoading=!0,c.sendCmd("i2cdetect",["-y","0"])},parseData:function(t){var e=[],n=!0,r=!1,i=void 0;try{for(var s,o=a()(t.split("\n"));!(n=(s=o.next()).done);n=!0){var c=s.value;if((c=c.split(":")).length>1){var d=!0,l=!1,u=void 0;try{for(var p,m=a()(c[1].split(" "));!(d=(p=m.next()).done);d=!0){var v=p.value;["UU","--"," ","","-","—"].indexOf(v)>-1||e.push(v)}}catch(t){l=!0,u=t}finally{try{!d&&m.return&&m.return()}finally{if(l)throw u}}}}}catch(t){r=!0,i=t}finally{try{!n&&o.return&&o.return()}finally{if(r)throw i}}return e},lookUpProduct:function(t){for(var e=0;e<s.length;e++)if(s[e].deviceAddr===t)return s[e];return null}}},C={render:function(){var t=this,e=t.$createElement,n=t._self._c||e;return n("div",{attrs:{id:"app"}},[n("div",{staticClass:"control"},[n("label",{staticClass:"form-switch float-right"},[n("input",{directives:[{name:"model",rawName:"v-model",value:t.isAutoRefresh,expression:"isAutoRefresh"}],attrs:{type:"checkbox","true-value":!0,"false-value":!1},domProps:{checked:Array.isArray(t.isAutoRefresh)?t._i(t.isAutoRefresh,null)>-1:t.isAutoRefresh},on:{change:function(e){var n=t.isAutoRefresh,r=e.target,i=!!r.checked;if(Array.isArray(n)){var a=t._i(n,null);r.checked?a<0&&(t.isAutoRefresh=n.concat([null])):a>-1&&(t.isAutoRefresh=n.slice(0,a).concat(n.slice(a+1)))}else t.isAutoRefresh=i}}}),t._v(" "),n("i",{staticClass:"form-icon"}),t._v("Auto Refresh\n    ")]),t._v(" "),t.isAutoRefresh?t._e():n("button",{staticClass:"refreshBtn btn float-right",on:{click:t.onRefresh}},[t._v("Refresh")])]),t._v(" "),n("div",{staticClass:"divider text-center",attrs:{"data-content":"Connected QWIIC devices will show up below"}}),t._v(" "),n("div",{staticClass:"productList"},t._l(t.productList,function(e,r){return n("div",{key:r,staticClass:"card productItem"},[n("div",{staticClass:"card-image productPhoto"},[n("img",{staticClass:"img-responsive",attrs:{src:e.imageUrl}})]),t._v(" "),n("div",{staticClass:"productName"},[n("div",{staticClass:"card-title h5"},[t._v(t._s(e.name))]),t._v(" "),n("div",{staticClass:"card-subtitle text-gray"},[t._v("I2C Addr ("+t._s(e.deviceAddr)+")")])]),t._v(" "),n("div",{staticClass:"productFooter"},[e.component?n("a",{staticClass:"btn btn-success float-right mt-2",on:{click:function(n){return t.interact(e)}}},[t._v("Interact")]):t._e(),t._v(" "),n("a",{staticClass:"btn btn-link float-right",attrs:{href:e.productUrl,target:"_blank"}},[t._v("Learn More")])])])}),0),t._v(" "),t.interactProduct?n("div",{staticClass:"modal active"},[n("a",{staticClass:"modal-overlay",on:{click:function(e){return t.interact(null)}}}),t._v(" "),n("div",{staticClass:"modal-container"},[n("div",{staticClass:"modal-header"},[n("a",{staticClass:"btn btn-clear float-right",on:{click:function(e){return t.interact(null)}}}),t._v(" "),n("div",{staticClass:"product-modal-title"},[n("img",{staticClass:"header-image",attrs:{src:t.interactProduct.imageUrl,alt:""}}),t._v(" "),n("div",{staticClass:"modal-title"},[n("div",{staticClass:"h5"},[t._v(t._s(t.interactProduct.name))]),t._v(" "),n("div",{staticClass:"text-gray"},[t._v("I2C Addr ("+t._s(t.interactProduct.deviceAddr)+")")])])])]),t._v(" "),n("div",{staticClass:"modal-body"},[n("div",{staticClass:"content"},[n(t.interactProduct.component,{ref:"activeProduct",tag:"component",on:{onCreated:t.onCreated,onDestroyed:t.onDestroyed}})],1)])])]):t._e()])},staticRenderFns:[]};var w=n("VU/8")(g,C,!1,function(t){n("5Foo")},null,null).exports;r.a.config.productionTip=!1,new r.a({el:"#app",components:{App:w},template:"<App/>"})},Qyra:function(t,e){},"Wq+4":function(t,e){},X8mX:function(t,e){},i8c9:function(t,e){},pSRK:function(t,e){}},["NHnr"]);
//# sourceMappingURL=app.4dc049a90b03801ba255.js.map