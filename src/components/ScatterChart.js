import React, { useRef, useEffect } from 'react';
import * as d3 from 'd3';

const ScatterChart = ({ coords, path }) => {
  const ref = useRef();
      console.log("path",path)
  console.log(coords)
  var data = []
  var first
  var bool = false
  for(let i = 0; i<path.length+1;++i)
{
   const coordinateObj = coords.find(obj => (obj.id === path[i]+1));
   // check if the object exists before accessing its properties
   if(coordinateObj != null && coordinateObj.coords != null){
    bool = true
    if(i<path.length)
    {
      const val = coordinateObj.coords
      data.push(val)
      if(i === 0)
      {
          first = val
        
      }
    }
   
      
   }
   if (bool===true && i ===path.length)
   {
   
      
      data.push(first)
    
   }
}




    // const data = [
    //   {x: -1, y: 6},
    //   {x: -3, y: 2},
    //   {x: 4, y: 5},
    //   {x: -6, y: 1},
    //   {x: -1, y: 6},
    //   // ... your data here
    // ];
  useEffect(() => {
  
    const svg = d3.select(ref.current);
  svg.selectAll("*").remove();

  const svgWidth = 1000;
  const svgHeight = 1000;
  const margin = { top: 100, right: 100, bottom: 100, left: 100 };
  const width = svgWidth - margin.left - margin.right;
  const height = svgHeight - margin.top - margin.bottom;

  // Updated scale to include full extent of data
  const xScale = d3.scaleLinear()
    .domain(d3.extent(data, d => d.x)) // d3.extent finds the min and max values in your data
    .range([0, width])
    .nice();

  const yScale = d3.scaleLinear()
    .domain(d3.extent(data, d => d.y)) // Same for y-axis
    .range([height, 0])
    .nice();

  const g = svg.append("g")
    .attr("transform", `translate(${margin.left},${margin.top})`);

  const line = d3.line()
    .x(d => xScale(d.x))
    .y(d => yScale(d.y));

  g.append('path')
    .data([data])
    .attr('fill', 'none')
    .attr('stroke', 'blue')
    .attr('stroke-width', 1.5)
    .attr('d', line);

  data.forEach(d => {
    g.append('circle')
      .attr('cx', xScale(d.x))
      .attr('cy', yScale(d.y))
      .attr('r', 5)
      .attr('fill', 'red');
  });

  // Adding X Axis
  svg.append("g")
    .attr("transform", `translate(${margin.left},${height + margin.top})`)
    .call(d3.axisBottom(xScale));

  // Adding Y Axis
  svg.append("g")
    .attr("transform", `translate(${margin.left},${margin.top})`)
    .call(d3.axisLeft(yScale));

  },[coords, path]);

  return (
    <div className="Scatter">
      <svg ref={ref} width={1000} height={1000} ></svg>
    </div>
  );
}
export default ScatterChart;




