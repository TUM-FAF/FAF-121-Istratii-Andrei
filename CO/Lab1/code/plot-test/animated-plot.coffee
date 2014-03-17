

$ ->

  # bind links
  $("#start").click -> startSimulation()

  $("#stop").click -> stopSimulation()


  margin = {top: 20, right: 20, bottom: 20, left: 20}
  width = 840 - margin.left - margin.right
  height = 240 - margin.top - margin.bottom


  svg = d3.select("#graph").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")")



  svg.append("defs").append("clipPath")
    .attr("id", "clip")
    .append("rect")
    .attr("width", width)
    .attr("height", height)


  svg.append("rect")
    .attr("width", width)
    .attr("height", height)
    .attr("fill", "none")
    .attr("stroke", "black")
    .attr("stroke-width", 0.5)


  n = 100

  xScale = d3.scale.linear()
    .domain([0,n-1])
    .range([0, width])


  yScale = d3.scale.linear()
    .domain([-5,5])
    .range([height, 0])


  svg.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + yScale(0) + ")")
    .call(d3.svg.axis().scale(xScale).orient("bottom"))

  svg.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale).orient("left"))


  line = d3.svg.line()
    .x( (d,i) -> xScale(i) )
    .y( (d,i) -> yScale(f(d)) )
    .interpolate("basis")

  f = Math.sin
  data = []
  data.push 0 while data.length < n


  path = svg.append("g")
    .attr("clip-path", "url(#clip)")
    .append("path")
    .attr("d", line(data))
    .attr("stroke", "blue")
    .attr("stroke-width", 2)
    .attr("fill", "none")



  simulationRunning = false

  startSimulation = () ->
    simulationRunning = true
    console.log "Simulation started"
    tick()

  stopSimulation = () ->
    simulationRunning = false
    console.log "Simulation ended"


  makeCounter = (delta) ->
    t = 0
    () ->
      r = t
      t += delta
      r

  timer = makeCounter(1)

  tick = () ->
    if simulationRunning
      t = timer()
      data.push(t)        
      console.log t
      path.attr("d", line(data))
          .attr("transform", null)
          .transition()
          .duration(50)
          .ease("linear")
          .attr("transform", "translate(" + xScale(-1) + ",0)")
          .each("end", tick)
      data.shift()





  null