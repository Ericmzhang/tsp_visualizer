import { useState } from "react"
import axios from 'axios'
import '../App.css'
import ScatterChart from "./ScatterChart"
import { index } from "d3"
function Manualinput ({inputType}){
    const [xVal, setXVal] = useState('')
    const [yVal, setYVal] = useState('')
    const [coords, setCoords] = useState([])
    const [path, setPath] = useState('')
    

    const addVal = (event)=>{
        event.preventDefault()
        const coordsObject = {
            x: Number(xVal),
            y: Number(yVal),
        }

        const obj = {
            id: coords.length+1,
            coords: coordsObject
        }
        
        setCoords(coords.concat(obj))
        setXVal('')
        setYVal('')

    }

    const handleXChange = (event) => {
        
        setXVal(event.target.value)
      }

      const handleYChange = (event) => {
        
        setYVal(event.target.value)
      }

      const callTspmanual = (coordStr)=>{
        // Now, you can send 'base64Data' to the server
        console.log("coordstr",coordStr)
        axios.post('http://localhost:3001/run-cpp-manual', {data:coordStr})
        .then(response => {
          console.log("Response:", response.data)
          readResult(response.data)
        });
      }

      const readResult = (data)=>{
        const lines = data.split('\n'); // This is the file content
        const cur_path = lines[1].split(' ').map(num => parseInt(num, 10))
        const new_path = cur_path.slice(0,cur_path.length-1)
        console.log(new_path)
        setPath(new_path)
    
  }

      const graphManual = () => {
        console.log(coords)
        axios.post('http://localhost:3001/data', coords)
        let coordStr = '';
        coordStr += `${coords.length} \n`
        coords.forEach((coord) => {
            coordStr += `${coord.coords.x} ${coord.coords.y}\n`;
        });
        coordStr += `\n`
   
        callTspmanual(coordStr)
       
      }


    const reset = () => {
        console.log("reset button")
        axios.post('http://localhost:3001/clear-data')
        setCoords([])
        setPath('')
    }

    if(inputType ==="manual")
    {
        return (
            <div>
                <p>Enter (x,y) coordinates. <br>
                </br>The first coordinate must be the starting point and the rest can be in any order</p>
                <div className="buttons">
                    <form onSubmit={addVal}>
                    X:<input className="input" value = {xVal} onChange = {handleXChange}/>
                    Y:<input className="input" value = {yVal} onChange = {handleYChange}/>
                    <button type="submit">add</button>
                    </form>  
                    
                </div>
                <ul >
                    {coords.map(coord =>
                        <p key = {coord.id} className="compact">x:{coord.coords.x} y:{coord.coords.y}</p>
                    )}
                </ul>
                <button onClick = {graphManual}>graph</button>
                <button onClick = {reset}>reset entries</button>
                <ScatterChart  className ='Scatter' coords = {coords} path = {path}></ScatterChart>
                
            </div>
        )
    }
    else 
    {
        return null
    }
}

export default Manualinput