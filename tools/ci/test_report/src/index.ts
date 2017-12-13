import Vue from 'vue'
import VueCharts from 'vue-chartjs'

declare var test_results : any;
declare var trend_results : any;

Vue.component('line-chart', {
    extends: VueCharts.Line,
    props: ['x', 'y', 'options'],
    mounted () {
      let that = <any>this;
      let labels = <string[]>[];      
      let data = {
          labels: that.x,
          datasets: [{
            backgroundColor: '#f87979',
            pointRadius : 1.5,
            pointHitRadius : 4,
              data:that.y
            }
        ]
      };
      let options = {
        legend : { display: false},
        scaleLineColor : "rgba(0,0,0,0)",
        scaleShowLabels : false,
        scaleShowGridLines : false,
        
        datasetFill : false,
        // Sadly if you set scaleFontSize to 0, chartjs crashes
        // Instead we'll set it as small as possible and make it transparent
        scaleFontSize : 1,
        scaleFontColor : "rgba(0,0,0,0)",
        responsive: true,
        maintainAspectRatio: false,
        scales: {
            xAxes: [{
                display: false
            }]
        }
      }

      that.renderChart(data, options);
    }
  })


function GET(url: string) {
    return new Promise<string>((resolve, reject) => {
        var rq = new XMLHttpRequest();
        rq.onreadystatechange = function () {
            if (this.readyState === XMLHttpRequest.DONE) {
                if (this.status === 200) {
                    resolve(this.responseText);
                } else {
                    reject("Error: " + this.status + " " + this.responseText);
                }
            }
        };        
        rq.withCredentials = true;
        rq.open("GET", url);
        rq.send();
    });
}


var vm = new Vue({
    el: '#app',
    data: {
        nrofclicks: 0,
        message: "hej",
        http_example: "",
        test_results: [],
        trend_xaxis : [],
        trend_results : trend_results.results,
        example_x : [20,30,40,50,60],
        example_y : [20,30,40,14,20,22],
        loadTestResultFailMessage : ""
    },
    methods: {
        increment() {
            let that = this;            

            let getLatestFromJenkins = function() {
                return GET("https://icup_android.jenkins.cm.volvocars.biz/job/ihu_hourly_test/lastBuild/buildNumber").then((buildIdStr) => {                
                    let buildToGet = parseInt(buildIdStr);
                    let url = `https://icup_android.jenkins.cm.volvocars.biz/job/ihu_hourly_test/${buildToGet}/artifacts/test_result.json`
    
                    return GET(url);                
                })
            };
            let getExampleResults = function() {
                return GET("./results/test_results.json");     
            }

            //let methodToUse = getLatestFromJenkins;
            let methodToUse = getExampleResults;

            methodToUse().then((buildTestResultJsonStr) => {      
                let test_results = JSON.parse(buildTestResultJsonStr);

                for (let i=0;i<test_results.results.length;i++) {
                    test_results.results[i].expanded = false;
                }
                
                that.test_results = test_results.results;
            }).catch((e) => {
                console.error(e);
                that.loadTestResultFailMessage = "Error: " + e
            });
        },
        expand(tc) { tc.expanded = !tc.expanded },
        getKpiTrends(trend_item) {
            let res = <any[]>[];
            for (let key in trend_item) {
                if (!trend_item.hasOwnProperty(key)) 
                    continue;
                if (!key.startsWith("kpi")) 
                    continue;
                res.push({
                    name: key.substring(4),
                    value: trend_item[key]
                });
            }
            return res;
        }
    },
    beforeMount() {
        this.increment();
    }
});