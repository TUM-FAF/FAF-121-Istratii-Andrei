// Generated by CoffeeScript 1.7.1
(function() {
  var Graph, Path, Spring, SpringAnimation, rk4;

  $(function() {
    var anim, extForcePath, graphs, isRunning, linScale, onResize, pauseSimulation, playSimulation, positionPath, process, resetSimulation, simulate, sliderScales, spring, startupParams, subdivs, velocityPath;
    $(".play_btn").click(function() {
      playSimulation();
      $(this).toggle();
      return $(".pause_btn").toggle();
    });
    $(".pause_btn").click(function() {
      pauseSimulation();
      $(this).toggle();
      return $(".play_btn").toggle();
    });
    $(".reset_btn").click(function() {
      return resetSimulation();
    });
    $(window).on("resize", function() {
      return onResize();
    });
    $("[data-slider]").on("change", function() {
      var data, param_name;
      param_name = $(this).attr("id").match(/(.+?)-slider/)[1];
      data = sliderScales[param_name]($(this).attr("data-slider"));
      return $("#" + param_name + "-feedback").html(data.toFixed(2));
    });
    isRunning = false;
    process = null;
    subdivs = 100;
    startupParams = {
      mass: 1,
      elasticity: 1,
      damping: 0.1,
      amplitude: 0,
      pulsation: 0,
      phase: 0,
      position: 1,
      velocity: 0,
      delta: 0.5
    };
    spring = new Spring(startupParams);
    graphs = [new Graph("graph1", subdivs), new Graph("graph3", subdivs)];
    positionPath = new Path("blue");
    velocityPath = new Path("red");
    extForcePath = new Path("blue");
    positionPath.fill(subdivs, spring.position);
    velocityPath.fill(subdivs, spring.velocity);
    extForcePath.fill(subdivs, spring.extForce(0));
    graphs[0].attachPath(positionPath);
    graphs[0].attachPath(velocityPath);
    graphs[1].attachPath(extForcePath);
    anim = new SpringAnimation("spring");
    anim.update(spring.position);
    $(".pause_btn").hide();
    linScale = function(from, to) {
      return d3.scale.linear().domain([0, 100]).range([from, to]);
    };
    sliderScales = {
      mass: linScale(0.1, 3.1),
      elasticity: linScale(0, 2),
      damping: linScale(0, 2),
      amplitude: linScale(-2, 2),
      pulsation: linScale(0, 4),
      phase: linScale(-Math.PI, Math.PI),
      position: linScale(-2, 2),
      velocity: linScale(0, 2),
      delta: linScale(0.2, 2)
    };
    $.each(startupParams, function(key, value) {
      return $("#" + key + "-slider").foundation("slider", "set_value", sliderScales[key].invert(value));
    });
    playSimulation = function() {
      console.log("Simulation started");
      if (!isRunning) {
        process = setInterval((function() {
          return simulate();
        }), 100);
      }
      return isRunning = true;
    };
    pauseSimulation = function() {
      clearInterval(process);
      isRunning = false;
      return console.log("Simulation ended");
    };
    resetSimulation = function() {
      var g, params, _i, _len;
      params = {};
      $("[data-slider]").each(function(i, element) {
        var param_name, slider;
        slider = $(element);
        param_name = slider.attr("id").match(/(.+?)-slider/)[1];
        return params[param_name] = sliderScales[param_name](slider.attr("data-slider"));
      });
      spring.reset(params);
      positionPath.fill(subdivs, spring.position);
      velocityPath.fill(subdivs, spring.velocity);
      extForcePath.fill(subdivs, spring.extForce(0));
      for (_i = 0, _len = graphs.length; _i < _len; _i++) {
        g = graphs[_i];
        g.updateState();
      }
      return anim.update(spring.position);
    };
    simulate = function() {
      var f, g, v, x, _i, _len, _ref;
      _ref = spring.next(), x = _ref[0], v = _ref[1], f = _ref[2];
      positionPath.data.push(x);
      velocityPath.data.push(v);
      extForcePath.data.push(f);
      for (_i = 0, _len = graphs.length; _i < _len; _i++) {
        g = graphs[_i];
        g.updateState();
        g.translate();
      }
      positionPath.data.shift();
      velocityPath.data.shift();
      extForcePath.data.shift();
      return anim.update(x);
    };
    return onResize = function() {
      var graph, _i, _len, _results;
      anim.resize();
      _results = [];
      for (_i = 0, _len = graphs.length; _i < _len; _i++) {
        graph = graphs[_i];
        _results.push(graph.resize());
      }
      return _results;
    };
  });

  Graph = (function() {
    function Graph(graphID, subdivisions) {
      var height, margin, w, width, xS, yS;
      this.graphID = graphID;
      this.paths = [];
      margin = {
        top: 10,
        right: 20,
        bottom: 10,
        left: 50
      };
      this.aspectRatio = 4;
      w = $("#" + this.graphID).width();
      width = w - margin.left - margin.right;
      height = w / this.aspectRatio - margin.top - margin.bottom;
      this.svg = d3.select("#" + this.graphID).append("svg").attr("width", width + margin.left + margin.right).attr("height", height + margin.top + margin.bottom).attr("viewBox", "0 0 " + (width + margin.left + margin.right) + " " + (height + margin.top + margin.bottom)).attr("preserveAspectRatio", "xMinYMin").append("g").attr("transform", "translate(" + margin.left + "," + margin.top + ")");
      this.svg.append("defs").append("clipPath").attr("id", "clip").append("rect").attr("width", width).attr("height", height);
      this.svg.append("rect").attr("width", width).attr("height", height).attr("fill", "none").attr("stroke", "black").attr("stroke-width", 0.5);
      this.xScale = d3.scale.linear().domain([0, subdivisions - 1]).range([0, width]);
      this.yScale = d3.scale.linear().domain([-3, 3]).range([height, 0]);
      xS = this.xScale;
      yS = this.yScale;
      this.lineGenerator = d3.svg.line().x(function(d, i) {
        return xS(i);
      }).y(function(d, i) {
        return yS(d);
      }).interpolate("basis");
      this.svg.append("g").attr("class", "x axis").attr("transform", "translate(0," + this.yScale(0) + ")").call(d3.svg.axis().scale(this.xScale).orient("bottom").ticks(0));
      this.svg.append("g").attr("class", "y axis").call(d3.svg.axis().scale(this.yScale).orient("left"));
    }

    Graph.prototype.attachPath = function(p) {
      this.paths.push(p);
      return this.svg.append(function() {
        return p.htmlNode.node();
      });
    };

    Graph.prototype.updateState = function() {
      var p, _i, _len, _ref, _results;
      _ref = this.paths;
      _results = [];
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        p = _ref[_i];
        _results.push(p.interpretData(this.lineGenerator));
      }
      return _results;
    };

    Graph.prototype.translate = function() {
      var p, _i, _len, _ref, _results;
      _ref = this.paths;
      _results = [];
      for (_i = 0, _len = _ref.length; _i < _len; _i++) {
        p = _ref[_i];
        _results.push(p.pathElement.attr("transform", null).transition().duration(50).ease("linear").attr("transform", "translate(" + this.xScale(-1) + ",0)"));
      }
      return _results;
    };

    Graph.prototype.resize = function() {
      var width;
      width = $("#" + this.graphID).width();
      return d3.select("#" + this.graphID + " svg").attr("width", width).attr("height", width / this.aspectRatio);
    };

    return Graph;

  })();

  Path = (function() {
    function Path(color) {
      var xmlns;
      this.data = [];
      xmlns = "http://www.w3.org/2000/svg";
      this.htmlNode = d3.select(document.createElementNS(xmlns, "g")).attr("clip-path", "url(#clip)");
      this.pathElement = this.htmlNode.append("svg:path").attr("stroke", color).attr("stroke-width", 2).attr("fill", "none");
    }

    Path.prototype.fill = function(n, d) {
      var _results;
      this.data.length = 0;
      _results = [];
      while (this.data.length < n) {
        _results.push(this.data.push(d));
      }
      return _results;
    };

    Path.prototype.interpretData = function(generator) {
      return this.pathElement.attr("d", generator(this.data));
    };

    return Path;

  })();

  Spring = (function() {
    function Spring(params) {
      this.reset(params);
    }

    Spring.prototype.reset = function(params) {
      this.t = 0.0;
      $.each(params, (function(_this) {
        return function(k, v) {
          return _this[k] = v;
        };
      })(this));
      return this.extForce = (function(_this) {
        return function(t) {
          return _this.amplitude * Math.sin(_this.pulsation * t + _this.phase);
        };
      })(this);
    };

    Spring.prototype.next = function() {
      var f, f1, f2, _ref;
      f1 = (function(_this) {
        return function(t, ys) {
          return ys[1];
        };
      })(this);
      f2 = (function(_this) {
        return function(t, ys) {
          return (_this.extForce(t) - _this.damping * ys[1] - _this.elasticity * ys[0]) / _this.mass;
        };
      })(this);
      _ref = rk4([f1, f2], this.delta, this.t, [this.position, this.velocity]), this.position = _ref[0], this.velocity = _ref[1];
      f = this.extForce(this.t);
      console.log("" + (this.t.toFixed(1)) + " -- " + (this.position.toFixed(3)) + " -- " + f);
      this.t += this.delta;
      return [this.position, this.velocity, f];
    };

    return Spring;

  })();

  rk4 = function(fs, h, t, ys) {
    var h2, k1, k2, k3, k4, scalarMul, u, v, vectorSum, w;
    vectorSum = function(v1, v2) {
      var i, v, _i, _len, _results;
      _results = [];
      for (i = _i = 0, _len = v1.length; _i < _len; i = ++_i) {
        v = v1[i];
        _results.push(v1[i] + v2[i]);
      }
      return _results;
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
      var cliper, height, margin, width;
      margin = {
        top: 0,
        right: 0,
        bottom: 20,
        left: 0
      };
      width = $("#" + graphID).width() - margin.left - margin.right;
      height = width;
      this.graphID = graphID;
      this.svg = d3.select("#" + graphID).append("svg").attr("width", width + margin.left + margin.right).attr("height", height + margin.top + margin.bottom).attr("viewBox", "0 0 " + (width + margin.left + margin.right) + " " + (height + margin.top + margin.bottom)).attr("preserveAspectRatio", "xMinYMin").append("g").attr("transform", "translate(" + margin.left + "," + margin.top + ")");
      this.svg.append("defs").append("clipPath").attr("id", "clip").append("rect").attr("width", width).attr("height", height);
      this.svg.append("rect").attr("width", width).attr("height", height).attr("fill", "none").attr("vector-effect", "non-scaling-stroke").attr("stroke", "#AAAAAA").attr("stroke-width", 0.4);
      this.xScale = d3.scale.linear().domain([0, 10]).range([0, width]);
      this.yScale = d3.scale.linear().domain([0, 10]).range([0, height]);
      this.lineGenerator = d3.svg.line().x(function(d) {
        return this.xScale(d[0]);
      }).y(function(d) {
        return this.yScale(d[1]);
      });
      this.initData = [[5, 0], [5, 0.75], [4, 1], [6, 1.5], [4, 2], [6, 2.5], [4, 3], [6, 3.5], [4, 4], [5, 4.25], [5, 6]];
      cliper = this.svg.append("g").attr("clip-path", "url(#clip)");
      this.path = cliper.append("path").attr("d", this.lineGenerator(this.initData)).attr("stroke", "blue").attr("stroke-width", 2).attr("fill", "none");
      this.circle = cliper.append("circle").attr("cx", this.xScale(5)).attr("cy", this.yScale(6)).attr("r", this.xScale(1));
    }

    SpringAnimation.prototype.resize = function() {
      var size;
      size = $("#" + this.graphID).width();
      return d3.select("#" + this.graphID + " svg").attr("width", size).attr("height", size);
    };

    SpringAnimation.prototype.update = function(pos) {
      this.circle.transition().duration(50).ease("linear").attr("transform", "translate(0, " + (this.yScale(pos * 2)) + ")");
      this.data = this.initData.map(function(p) {
        return [p[0], p[1] + 2 * pos * (p[1] / 6.0)];
      });
      return this.path.transition().duration(50).ease("linear").attr("d", this.lineGenerator(this.data));
    };

    return SpringAnimation;

  })();

}).call(this);
