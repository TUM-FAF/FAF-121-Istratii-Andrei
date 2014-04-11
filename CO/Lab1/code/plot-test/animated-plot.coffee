

$ ->

  # bind links
  $("#start").click ->
    startSimulation()
    #$(this).addClass("disabled")

  $("#stop").click -> stopSimulation()

  $(window).on "resize", -> onResize()
  
  isRunning = false
  process = null
  counter = makeCounter(0.5)

  spring = new Spring
  spring.reset(0, 0)

  graphs = []
  graphs.push new Graph "graph1", 0.5 # x
  #@graphs.push new Graph "graph2", 800, 200, 0.5 # v
  graphs.push new Graph "graph3", 0.5 # F(t)

  
  positionPath = new Path "blue"
  velocityPath = new Path "red"
  extForce = new Path "blue"

  graphs[0].attachPath positionPath
  graphs[0].attachPath velocityPath
  graphs[1].attachPath extForce

  anim = new SpringAnimation "spring"

  startSimulation = () ->
    console.log "Simulation started"
    process = setInterval((() -> simulate()), 100) unless isRunning
    isRunning = true

  stopSimulation = () ->
    clearInterval process
    isRunning = false
    console.log "Simulation ended"

  resetSimulation = () ->

  onResize = () ->
    anim.resize()
    for graph in graphs
      graph.resize()


  simulate = () ->
    t = counter()

    [x, v] = spring.next(0.5)

    positionPath.data.push(x)
    velocityPath.data.push(v)
    extForce.data.push(spring.externalForce(t-0.5))

    for g in graphs
      g.updateState()
      g.translate()
    
    positionPath.data.shift()
    velocityPath.data.shift()
    extForce.data.shift()

    anim.update(x)



class Graph
  constructor: (graphID, delta) ->

    @graphID = graphID

    @paths = []

    @n = 100
    margin = {top: 10, right: 20, bottom: 10, left: 50}

    @aspectRatio = 3

    w = $("##{@graphID}").width()

    width = w - margin.left - margin.right
    height = w/@aspectRatio - margin.top - margin.bottom

    @svg = d3.select("##{@graphID}").append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .attr('viewBox', '0 0 '+ (width + margin.left + margin.right) + ' ' + (height + margin.top + margin.bottom))
      .attr('preserveAspectRatio', 'xMinYMin')
      .append("g")
      .attr("transform", "translate(" + margin.left + "," + margin.top + ")")


    # clipping rect
    @svg.append("defs").append("clipPath")
      .attr("id", "clip")
      .append("rect")
      .attr("width", width)
      .attr("height", height)


    # border
    @svg.append("rect")
      .attr("width", width)
      .attr("height", height)
      .attr("fill", "none")
      .attr("stroke", "black")
      .attr("stroke-width", 0.5)

    @xScale = d3.scale.linear()
      .domain([0, @n-1])
      .range([0, width])


    @yScale = d3.scale.linear()
      .domain([-2,2])             # TODO: support changes
      .range([height, 0])

    # for closures in line generators
    xS = @xScale
    yS = @yScale

    @lineGenerator = d3.svg.line()
      .x( (d,i) -> xS(i) )
      .y( (d,i) -> yS(d) )
      .interpolate("basis")



    # x axis
    @svg.append("g")
      .attr("class", "x axis")
      .attr("transform", "translate(0," + @yScale(0) + ")")
      .call(d3.svg.axis().scale(@xScale).orient("bottom").ticks(0))

    # y axis
    @svg.append("g")
      .attr("class", "y axis")
      .call(d3.svg.axis().scale(@yScale).orient("left"))


  attachPath: (p) ->
    while p.data.length < @n
      p.data.push(0)

    @paths.push(p)
    @svg.append(() -> p.htmlNode.node())


  updateState: () ->
    for p in @paths
      p.interpretData(@lineGenerator)

  translate: () ->
    for p in @paths
      p.pathElement.attr("transform", null)
        .transition()
        .duration(50)
        .ease("linear")
        .attr("transform", "translate(" + @xScale(-1) + ",0)")

  resize: () ->
    width = $("##{@graphID}").width()
    console.log width
    d3.select("##{@graphID} svg")
      .attr("width", width)
      .attr("height", width/@aspectRatio)




class Path
  constructor: (color) ->

    @data = []    

    @htmlNode = d3.select(document.createElement("g"))
      .attr("clip-path", "url(#clip)")
      
    @pathElement = @htmlNode.append("path")
      .attr("stroke", color)
      .attr("stroke-width", 2)
      .attr("fill", "none")

  interpretData: (generator) ->
    @pathElement.attr("d", generator(@data))



class Spring
  constructor: (@mass = 1.0, @elasticity = 1.0, @damping = 0.0, @externalForce = ((t) -> 0.0)) ->
    @mass = 0.1
    @elasticity = 0.0
    @damping = 0.2
    @externalForce = (t) -> 0.3*Math.cos(t*3.0)

  reset: (x0, v0) ->
    @t = 0.0
    @x = x0
    @v = v0


  next: (delta) ->
    m = @mass
    k = @elasticity
    c = @damping
    f = @externalForce
    f1 = (t, ys) -> ys[1]
    f2 = (t, ys) -> (f(t) - c*ys[1] - k*ys[0])/m

    [@x, @v] = rk4([f1, f2], delta, @t, [@x, @v])
    @t += delta
    [@x, @v]


makeCounter = (delta) ->
  t = 0
  () ->
    r = t
    t += delta
    r


rk4 = (fs, h, t, ys) ->
  vectorSum = (v1, v2) ->
    res = []
    res.push(v1[i] + v2[i]) for v, i in v1
    res
  scalarMul = (v, a) -> v.map((x)->x*a)

  h2 = 0.5 * h
  k1 = fs.map( (f) -> f(t, ys) )
  k2 = fs.map( (f) -> f(t + h2, vectorSum(ys, scalarMul(k1,h2))) )
  k3 = fs.map( (f) -> f(t + h2, vectorSum(ys, scalarMul(k2,h2))) )
  k4 = fs.map( (f) -> f(t + h, vectorSum(ys, scalarMul(k3,h))) )
  u = scalarMul(k2, 2)
  v = scalarMul(k3, 2)
  w = vectorSum(vectorSum(vectorSum(k1, u), v), k4)  # inefficient but should work
  vectorSum(ys, scalarMul(w, (1/6.0)*h))



class SpringAnimation
  constructor: (graphID) ->
    margin = {top: 5, right: 5, bottom: 5, left: 5}

    width = $("##{graphID}").width() - margin.left - margin.right
    height = width

    @graphID = graphID

    @svg = d3.select("##{graphID}").append("svg")
      .attr("width", width + margin.left + margin.right)
      .attr("height", height + margin.top + margin.bottom)
      .attr('viewBox', '0 0 '+ (width + margin.left + margin.right) + ' ' + (height + margin.top + margin.bottom))
      .attr('preserveAspectRatio', 'xMinYMin')
      .append("g")
      .attr("transform", "translate(" + margin.left + "," + margin.top + ")")


    # clipping rect
    @svg.append("defs").append("clipPath")
      .attr("id", "clip")
      .append("rect")
      .attr("width", width)
      .attr("height", height)


    # border
    @svg.append("rect")
      .attr("width", width)
      .attr("height", height)
      .attr("fill", "none")
      .attr("vector-effect", "non-scaling-stroke")
      .attr("stroke", "#AAAAAA")
      .attr("stroke-width", 0.4)

    @xScale = d3.scale.linear()
      .domain([0, 10])
      .range([0, width])

    @yScale = d3.scale.linear()
      .domain([0, 10])             # TODO: support changes
      .range([0, height])



    @lineGenerator = d3.svg.line()
      .x( (d) -> @xScale(d[0]) )
      .y( (d) -> @yScale(d[1]) )

    ###
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
    ###

    @initData = [
      [5, 0],
      [5, 0.75],
      [4, 1],
      [6, 1.5],
      [4, 2],
      [6, 2.5],
      [4, 3],
      [6, 3.5],
      [4, 4],
      [5, 4.25],
      [5, 6]
    ]



    @path = @svg.append("g")
      .attr("clip-path", "url(#clip)")
      .append("path")
      .attr("d", @lineGenerator(@initData))
      .attr("stroke", "blue")
      .attr("stroke-width", 2)
      .attr("fill", "none")


    @circle = @svg.append("circle")
      .attr("cx", @xScale(5))
      .attr("cy", @yScale(6))
      .attr("r", @xScale(1))


  resize: () ->
    size = $("##{@graphID}").width()
    console.log size
    d3.select("##{@graphID} svg")
      .attr("width", size)
      .attr("height", size)


  update: (delta) ->
    @circle.transition()
        .duration(50)
        .ease("linear")
        .attr("transform", "translate(0, " + (@yScale(delta*2)) + ")")

    @data = @initData.map( (p) ->
      [x, y] = p
      [x, (y + 2*delta*(y/6.0))]
    )

    @path.transition()
      .duration(50)
      .ease("linear")
      .attr("d", @lineGenerator(@data))