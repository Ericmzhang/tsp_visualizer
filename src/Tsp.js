import React, { useState } from 'react';
import axios from 'axios'
import './Tsp.css'
import Textinput from './components/Textinput';
import Manualinput from './components/Manualinput';
import { Link } from 'react-router-dom';
import styles from './Tsp.module.css'; 



function Tsp() {
  const [fileContent, setFileContent] = useState('')
  const [Coords, setCoords] = useState([])
  const [Path, setPath] = useState('')
  const [inputType, setInputType] = useState('')

  const handleFileChange = (event) =>{
    const file = event.target.files[0];
    if (!file) {
      return;
    }
  
    readTextFile(file);
    callTsptext(file)
  }

  const callTsptext = (file) =>{
    console.log("file",file)
    const formData = new FormData();
    formData.append('file', file);
    axios.post('http://localhost:3001/run-cpp-txt', formData, {
      headers: {
        'Content-Type': 'multipart/form-data',
      },
        })
        .then(response => {
           console.log("response",response.data)
           readResult(response.data)
        });
        
  }

 
  

  const readTextFile = (file) =>{
    const reader = new FileReader();
    reader.onload = function(e) {
      setFileContent(e.target.result);
      const lines = e.target.result.split('\n');
      var temp = []
      for(let i =0;i <lines.length-1;++i)
      {
        if(i>0){
          const parts = lines[i].split(" ");
          const num1 = parseInt(parts[0]); // Convert the first part to an integer
        const num2 = parseInt(parts[1]); 
        const coords= {
          x: num1,
          y: num2
        }

        const newObj = {
          id: i,
          coords: coords
        }
          temp = [...temp, newObj]
          
        }
      }
      setCoords(temp)
      axios.post('http://localhost:3001/data', temp)
      // .then(response => {
      //   console.log(response)
      // })

      
    };
  
    reader.onerror = function(e) {
      console.error("Error reading file", e);
    };
  
    reader.readAsText(file); // Read the file as text
  }

const readResult = (data)=>{
      const lines = data.split('\n'); // This is the file content
      const cur_path = lines[1].split(' ').map(num => parseInt(num, 10))
      const new_path = cur_path.slice(0,cur_path.length-1)
      console.log(new_path)
      setPath(new_path)
  
}

const setType = (type)=>{
  axios.post('http://localhost:3001/clear-data')
  setInputType(type)
}


  return (
    <div >
    <div className='Left'>
      <Link to ="/">Home</Link>
      <div className = "buttons">
        <button className="square-button" onClick={() => setType("file")}>txt File Input</button>
        <button className="square-button" onClick={() => setType("manual")}>Manual Input</button>
      </div>
      
      <Textinput handleFileChange={handleFileChange} inputType ={inputType} coords = {Coords} path = {Path} fileContent={fileContent}></Textinput>
      <Manualinput inputType = {inputType}></Manualinput>
      
      
      
    </div>
    </div>
  );
}

export default Tsp;
