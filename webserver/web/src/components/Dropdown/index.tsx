import React, { useState } from "react";
import {MultiSelect} from "react-multi-select-component";

const Example: React.FC = () => {
  const options = [
    { label: "Tamanho P", value: "p" },
    { label: "Tamanho M", value: "m" },
    { label: "Tamanho G", value: "g" },
  ];

  const [selected, setSelected] = useState([]);

  return (
    <div>
      <h2>Selecione o Dispositivo</h2>
      <pre>{JSON.stringify(selected)}</pre>
      <MultiSelect
        options={options}
        value={selected}
        onChange={setSelected}
        labelledBy={"Select"}
      />
    </div>
  );
};

export default Example;