// Generated by CoffeeScript 1.7.1
(function() {
  var Graph, Path, Simulation, Spring, SpringAnimation, makeCounter, rk4;

  $(function() {
    var simulation;
    simulation = new Simulation;
    $("#start").click(function() {
      return simulation.startSimulation();
    });
    $("#stop").click(function() {
      return simulation.stopSimulation();
    });
    return $(window).on("resize", function() {
      return simulation.onResize();
    });
  });

  Simulation = (function() {
    function Simulation() {
      this.isRunning = false;
      this.counter = makeCounter(0.5);
      this.spring = new Spring;
      this.spring.reset(0, 0);
      this.graphs = [];
      this.graphs.push(new Graph("graph1", 0.5));
      this.graphs.push(new Graph("graph3", 0.5));
      this.paths = [];
      this.paths.push(new Path(this.graphs[0], "blue"));
      this.paths.push(new Path(this.graphs[0], "red"));
      this.paths.push(new Path(this.graphs[1], "blue"));
      this.anim = new SpringAnimation("spring");
    }

    Simulation.prototype.startSimulation = function() {
      var that;
      console.log("Simulation started");
      that = this;
      if (!this.isRunning) {
        this.process = setInterval((function() {
          return that.simulate();
        }), 100);
      }
      return this.isRunning = true;
    };

    Simulation.prototype.stopSimulation = function() {
      clearInterval(this.process);
      this.isRunning = false;
      return console.log("Simulation ended");
    };

    Simulation.prototype.resetSimulation = function() {};

    Simulation.prototype.onResize = function() {
      var graph, _i, _len, _ref, _results;
      this.anim.resize();
      _ref = this.graphs;
      _results = [];
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        graph = _ref[_i];
        _results.push(graph.resize());
      }
      return _results;
    };

    Simulation.prototype.simulate = function() {
      var path, t, v, x, _i, _len, _ref, _ref1, _results;
      t = this.counter();
      _ref = this.spring.next(0.5), x = _ref[0], v = _ref[1];
      this.paths[0].data.push(x);
      this.paths[1].data.push(v);
      this.paths[2].data.push(this.spring.externalForce(t - 0.5));
      _ref1 = this.paths;
      _results = [];
      for (_i = 0, _len = _ref1.length; _i < _len; _i++) {
        path = _ref1[_i];
        path.path.attr("d", path.lineGenerator(path.data)).attr("transform", null).transition().duration(50).ease("linear").attr("transform", "translate(" + this.graphs[0].xScale(-1) + ",0)");
        path.data.shift();
        _results.push(this.anim.update(x));
      }
      return _results;
    };

    return Simulation;

  })();

  Graph = (function() {
    function Graph(graphID, delta) {
      var aspectRatio, height, margin, n, w, width;
      this.graphID = graphID;
      n = 100;
      margin = {
        top: 10,
        right: 20,
        bottom: 10,
        left: 50
      };
      aspectRatio = 3;
      w = $("#" + this.graphID).width();
      width = w - margin.left - margin.right;
      height = w / aspectRatio - margin.top - margin.bottom;
      this.svg = d3.select("#" + this.graphID).append("svg").attr("width", width + margin.left + margin.right).attr("height", height + margin.top + margin.bottom).attr('viewBox', '0 0 ' + (width + margin.left + margin.right) + ' ' + (height + margin.top + margin.bottom)).attr('preserveAspectRatio', 'xMinYMin').append("g").attr("transform", "translate(" + margin.left + "," + margin.top + ")");
      this.svg.append("defs").append("clipPath").attr("id", "clip").append("rect").attr("width", width).attr("height", height);
      this.svg.append("rect").attr("width", width).attr("height", height).attr("fill", "none").attr("stroke", "black").attr("stroke-width", 0.5);
      this.xScale = d3.scale.linear().domain([0, n - 1]).range([0, width]);
      this.yScale = d3.scale.linear().domain([-2, 2]).range([height, 0]);
      this.svg.append("g").attr("class", "x axis").attr("transform", "translate(0," + this.yScale(0) + ")").call(d3.svg.axis().scale(this.xScale).orient("bottom").ticks(0));
      this.svg.append("g").attr("class", "y axis").call(d3.svg.axis().scale(this.yScale).orient("left"));
    }

    Graph.prototype.resize = function() {
      var size;
      size = $("#" + this.graphID).width();
      console.log(size);
      return d3.select("#" + this.graphID + " svg").attr("width", size).attr("height", size / 3);
    };

    return Graph;

  })();

  Path = (function() {
    function Path(graph, color) {
      var n;
      n = 100;
      this.data = [];
      while (this.data.length < n) {
        this.data.push(0);
      }
      this.lineGenerator = d3.svg.line().x(function(d, i) {
        return graph.xScale(i);
      }).y(function(d, i) {
        return graph.yScale(d);
      }).interpolate("basis");
      this.path = graph.svg.append("g").attr("clip-path", "url(#clip)").append("path").attr("d", this.lineGenerator(this.data)).attr("stroke", color).attr("stroke-width", 2).attr("fill", "none");
    }

    return Path;

  })();

  Spring = (function() {
    function Spring(mass, elasticity, damping, externalForce) {
      this.mass = mass != null ? mass : 1.0;
      this.elasticity = elasticity != null ? elasticity : 1.0;
      this.damping = damping != null ? damping : 0.0;
      this.externalForce = externalForce != null ? externalForce : (function(t) {
        return 0.0;
      });
      this.mass = 0.1;
      this.elasticity = 0.0;
      this.damping = 0.2;
      this.externalForce = function(t) {
        return 0.3 * Math.cos(t * 3.0);
      };
    }

    Spring.prototype.reset = function(x0, v0) {
      this.t = 0.0;
      this.x = x0;
      return this.v = v0;
    };

    Spring.prototype.next = function(delta) {
      var c, f, f1, f2, k, m, _ref;
      m = this.mass;
      k = this.elasticity;
      c = this.damping;
      f = this.externalForce;
      f1 = function(t, ys) {
        return ys[1];
      };
      f2 = function(t, ys) {
        return (f(t) - c * ys[1] - k * ys[0]) / m;
      };
      _ref = rk4([f1, f2], delta, this.t, [this.x, this.v]), this.x = _ref[0], this.v = _ref[1];
      this.t += delta;
      return [this.x, this.v];
    };

    return Spring;

  })();

  makeCounter = function(delta) {
    var t;
    t = 0;
    return function() {
      var r;
      r = t;
      t += delta;
      return r;
    };
  };

  rk4 = function(fs, h, t, ys) {
    var h2, k1, k2, k3, k4, scalarMul, u, v, vectorSum, w;
    vectorSum = function(v1, v2) {
      var i, res, v, _i, _len;
      res = [];
      for (i = _i = 0, _len = v1.length; _i < _len; i = ++_i) {
        v = v1[i];
        res.push(v1[i] + v2[i]);
      }
      return res;
    };
    scalarMul = function(v, a) {
      return v.map(function(x) {
        return x * a;
      });
    };
    h2 = 0.5 * h;
    k1 = fs.map(function(f) {
      return f(t, ys);
    });
    k2 = fs.map(function(f) {
      return f(t + h2, vectorSum(ys, scalarMul(k1, h2)));
    });
    k3 = fs.map(function(f) {
      return f(t + h2, vectorSum(ys, scalarMul(k2, h2)));
    });
    k4 = fs.map(function(f) {
      return f(t + h, vectorSum(ys, scalarMul(k3, h)));
    });
    u = scalarMul(k2, 2);
    v = scalarMul(k3, 2);
    w = vectorSum(vectorSum(vectorSum(k1, u), v), k4);
    return vectorSum(ys, scalarMul(w, (1 / 6.0) * h));
  };

  SpringAnimation = (function() {
    function SpringAnimation(graphID) {
      var height, margin, width;
      margin = {
        top: 5,
        right: 5,
        bottom: 5,
        left: 5
      };
      width = $("#" + graphID).width() - margin.left - margin.right;
      height = width;
      this.graphID = graphID;
      this.svg = d3.select("#" + graphID).append("svg").attr("width", width + margin.left + margin.right).attr("height", height + margin.top + margin.bottom).attr('viewBox', '0 0 ' + (width + margin.left + margin.right) + ' ' + (height + margin.top + margin.bottom)).attr('preserveAspectRatio', 'xMinYMin').append("g").attr("transform", "translate(" + margin.left + "," + margin.top + ")");
      this.svg.append("defs").append("clipPath").attr("id", "clip").append("rect").attr("width", width).attr("height", height);
      this.svg.append("rect").attr("width", width).attr("height", height).attr("fill", "none").attr("vector-effect", "non-scaling-stroke").attr("stroke", "#AAAAAA").attr("stroke-width", 0.4);
      this.xScale = d3.scale.linear().domain([0, 10]).range([0, width]);
      this.yScale = d3.scale.linear().domain([0, 10]).range([0, height]);
      this.lineGenerator = d3.svg.line().x(function(d) {
        return this.xScale(d[0]);
      }).y(function(d) {
        return this.yScale(d[1]);
      });

      /*
      @data = [
        {x: 5, y: 0},
        {x: 5, y: 0.75},
        {x: 4, y: 1},
        {x: 6, y: 1.5},
        {x: 4, y: 2},
        {x: 6, y: 2.5},
        {x: 4, y: 3},
        {x: 6, y: 3.5},
        {x: 4, y: 4},
        {x: 5, y: 4.25}
        {x: 5, y: 6},
      ]
       */
      this.initData = [[5, 0], [5, 0.75], [4, 1], [6, 1.5], [4, 2], [6, 2.5], [4, 3], [6, 3.5], [4, 4], [5, 4.25], [5, 6]];
      this.path = this.svg.append("g").attr("clip-path", "url(#clip)").append("path").attr("d", this.lineGenerator(this.initData)).attr("stroke", "blue").attr("stroke-width", 2).attr("fill", "none");
      this.circle = this.svg.append("circle").attr("cx", this.xScale(5)).attr("cy", this.yScale(6)).attr("r", this.xScale(1));
    }

    SpringAnimation.prototype.resize = function() {
      var size;
      size = $("#" + this.graphID).width();
      console.log(size);
      return d3.select("#" + this.graphID + " svg").attr("width", size).attr("height", size);
    };

    SpringAnimation.prototype.update = function(delta) {
      this.circle.transition().duration(50).ease("linear").attr("transform", "translate(0, " + (this.yScale(delta * 2)) + ")");
      this.data = this.initData.map(function(p) {
        var x, y;
        x = p[0], y = p[1];
        return [x, y + 2 * delta * (y / 6.0)];
      });
      return this.path.transition().duration(50).ease("linear").attr("d", this.lineGenerator(this.data));
    };

    return SpringAnimation;

  })();

}).call(this);
