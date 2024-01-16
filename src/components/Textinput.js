import ScatterChart from "./ScatterChart";
import FileContent from "./FileContent";
function Textinput({handleFileChange, inputType, coords, path, fileContent}){
  // console.log("type",inputType)
  console.log("path",path)
  if(inputType ==="file")
  {
    
    return (
        <div>
        <div className='Flex'>
      <p> input a txt file in the following format: <br></br>
        number of coordinates <br></br>
        starting x y <br></br>
        other x y (in any order)<br></br>
        other x y (in any order)<br></br>
        end file with a space <br></br>
      </p>  
      <p className='Lefter'> example: <br></br>
      4  <br></br>
      1  3 <br></br>
      2  1  <br></br>
      4  2  <br></br>
      6  -1  <br></br>
      </p>
      </div>
      <input type="file" onChange={handleFileChange} />
      <FileContent fileContent = {fileContent} inputType={inputType}></FileContent>
      <ScatterChart coords = {coords} path = {path}></ScatterChart>
      </div>
    )
}
else{
  return null
}
}

export default Textinput;