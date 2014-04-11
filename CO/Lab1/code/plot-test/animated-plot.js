// Generated by CoffeeScript 1.7.1
(function() {
  var Graph, Path, Spring, SpringAnimation, makeCounter, rk4;

  $(function() {
    var anim, counter, extForce, graphs, isRunning, onResize, positionPath, process, resetSimulation, simulate, spring, startSimulation, stopSimulation, velocityPath;
    $("#start").click(function() {
      return startSimulation();
    });
    $("#stop").click(function() {
      return stopSimulation();
    });
    $(window).on("resize", function() {
      return onResize();
    });
    isRunning = false;
    process = null;
    counter = makeCounter(0.5);
    spring = new Spring;
    spring.reset(0, 0);
    graphs = [];
    graphs.push(new Graph("graph1", 0.5));
    graphs.push(new Graph("graph3", 0.5));
    positionPath = new Path("blue");
    velocityPath = new Path("red");
    extForce = new Path("blue");
    graphs[0].attachPath(positionPath);
    graphs[0].attachPath(velocityPath);
    graphs[1].attachPath(extForce);
    anim = new SpringAnimation("spring");
    startSimulation = function() {
      console.log("Simulation started");
      if (!isRunning) {
        process = setInterval((function() {
          return simulate();
        }), 100);
      }
      return isRunning = true;
    };
    stopSimulation = function() {
      clearInterval(process);
      isRunning = false;
      return console.log("Simulation ended");
    };
    resetSimulation = function() {};
    onResize = function() {
      var graph, _i, _len, _results;
      anim.resize();
      _results = [];
      for (_i = 0, _len = graphs.length; _i < _len; _i++) {
        graph = graphs[_i];
        _results.push(graph.resize());
      }
      return _results;
    };
    return simulate = function() {
      var g, t, v, x, _i, _len, _ref;
      t = counter();
      _ref = spring.next(0.5), x = _ref[0], v = _ref[1];
      positionPath.data.push(x);
      velocityPath.data.push(v);
      extForce.data.push(spring.externalForce(t - 0.5));
      for (_i = 0, _len = graphs.length; _i < _len; _i++) {
        g = graphs[_i];
        g.updateState();
        g.translate();
      }
      positionPath.data.shift();
      velocityPath.data.shift();
      extForce.data.shift();
      return anim.update(x);
    };
  });

  Graph = (function() {
    function Graph(graphID, delta) {
      var height, margin, w, width, xS, yS;
      this.graphID = graphID;
      this.paths = [];
      this.n = 100;
      margin = {
        top: 10,
        right: 20,
        bottom: 10,
        left: 50
      };
      this.aspectRatio = 3;
      w = $("#" + this.graphID).width();
      width = w - margin.left - margin.right;
      height = w / this.aspectRatio - margin.top - margin.bottom;
      this.svg = d3.select("#" + this.graphID).append("svg").attr("width", width + margin.left + margin.right).attr("height", height + margin.top + margin.bottom).attr('viewBox', '0 0 ' + (width + margin.left + margin.right) + ' ' + (height + margin.top + margin.bottom)).attr('preserveAspectRatio', 'xMinYMin').append("g").attr("transform", "translate(" + margin.left + "," + margin.top + ")");
      this.svg.append("defs").append("clipPath").attr("id", "clip").append("rect").attr("width", width).attr("height", height);
      this.svg.append("rect").attr("width", width).attr("height", height).attr("fill", "none").attr("stroke", "black").attr("stroke-width", 0.5);
      this.xScale = d3.scale.linear().domain([0, this.n - 1]).range([0, width]);
      this.yScale = d3.scale.linear().domain([-2, 2]).range([height, 0]);
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
      while (p.data.length < this.n) {
        p.data.push(0);
      }
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
      console.log(width);
      return d3.select("#" + this.graphID + " svg").attr("width", width).attr("height", width / this.aspectRatio);
    };

    return Graph;

  })();

  Path = (function() {
    function Path(color) {
      this.data = [];
      this.htmlNode = d3.select(document.createElement("g")).attr("clip-path", "url(#clip)");
      this.pathElement = this.htmlNode.append("path").attr("stroke", color).attr("stroke-width", 2).attr("fill", "none");
    }

    Path.prototype.interpretData = function(generator) {
      return this.pathElement.attr("d", generator(this.data));
    };

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
