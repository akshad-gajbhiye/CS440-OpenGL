// Vertex class.
export class Vertex 
{
    constructor(coords) 
    {
       this.coords = coords;
    }
}

// Function to flatten a vertex into a single array of numbers.
function flattenVertex(v)
{
    let vArr = [];
    for (let i = 0; i < 4; i++) vArr = vArr.concat(v.coords[i]);
    return vArr;
}

// Function to flatten an array of vertices into a single array of numbers.
export function flattenArrayOfVertices(a)
{
    let aArr = [];
    for (let i = 0; i < a.length; i++) aArr = aArr.concat(flattenVertex(a[i]));
    let aArrFl32 = new Float32Array(aArr);
    return aArrFl32;
}