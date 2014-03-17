

###
if window.performance.now
  console.log "Using high performance timer"
  getTimeStamp = () -> window.performance.now()
else
  if window.performance.webkitNow
    console.log "Using webkit high performance timer"
    getTimeStamp = () -> window.performance.webkitNow()
  else
    console.log "Using low performance timer"
    getTimeStamp = () -> new Date().getTime()
###



rk4v = (fs, h, t, ys) ->
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


rk4 = (f, h, t, ys) ->
  h2 = 0.5 * h
  k1 = f(t, ys)
  k2 = f(t + h2, )
  




plot = (graphID, xData, yData, width, height) ->
  margin = {top: 100, right: 100, bottom: 100, left: 100}


  svg = d3.select("##{graphID}").append("svg")
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
 
  

  xScale = d3.scale.linear()
    .domain([d3.min(xData), d3.max(xData)])
    .range([0, width])


  yScale = d3.scale.linear()
    .domain([d3.min(yData), d3.max(yData)])
    .range([height, 0])


  svg.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + yScale(0) + ")")
    .call(d3.svg.axis().scale(xScale).orient("bottom"))

  svg.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale).orient("left"))


  line = d3.svg.line()
    .x( (d,i) -> xScale(xData[i]) )
    .y( (d,i) -> yScale(yData[i]) )
    .interpolate("basis")

  path = svg.append("g")
    .attr("clip-path", "url(#clip)")
    .append("path")
    .attr("d", line(xData))
    .attr("stroke", "blue")
    .attr("stroke-width", 2)
    .attr("fill", "none")


$ ->


  xs = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]
  ys = [[0, 1]]

  g = (x) -> Math.cos x

  fs = [
    (t, ys) -> ys[0],
    (t, ys) -> ys[1]
  ]



  ys.push(rk4(fs, 1, xs[i], [].concat ys[i])) for x, i in xs
  ys = ys.map( (a) -> a[1] )
  zs = xs.map( (x) -> 2 - Math.cos(x) )

  console.log ys
  console.log zs

  plot("graph1", xs, ys, 800, 400)
  plot("graph2", xs, zs, 800, 400)
  
