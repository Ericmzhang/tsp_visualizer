function FileContent({fileContent,inputType}){
    if(inputType ==='file')
    {
        return(
        <div>
        <pre>{fileContent}</pre>
      </div>)
    }
    return null
}

 export default FileContent