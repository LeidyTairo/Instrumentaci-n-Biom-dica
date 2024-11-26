import React, { useState, useEffect } from "react";
import { View, Text, StyleSheet, Dimensions, ScrollView } from "react-native";
import { initializeApp, getApps } from "firebase/app";
import { getFirestore, doc, onSnapshot } from "firebase/firestore";
import { LineChart } from "react-native-chart-kit";

// Configuración de Firebase
const firebaseConfig = {
  apiKey: "AIzaSyDahsleW4BOMdoqPThTkhND2m6gkxKtc40",
  authDomain: "smartbeat-8548b.firebaseapp.com",
  projectId: "smartbeat-8548b",
  storageBucket: "smartbeat-8548b.appspot.com",
  messagingSenderId: "509681117394",
  appId: "1:509681117394:web:007f7feaa03dc36d02250b",
};

// Verifica si ya existe una instancia de Firebase antes de inicializar
if (!getApps().length) {
  initializeApp(firebaseConfig);
}

const db = getFirestore();

const MAX_DATA_POINTS = 2000; // Tamaño máximo del array para almacenar datos
const WINDOW_SIZE = 50; // Tamaño de la ventana deslizante para el gráfico

const Tutorial = () => {
  const [ecg1Data, setEcg1Data] = useState([]); // Array para los datos de ECG1
  const [timestamps, setTimestamps] = useState([]); // Array para los timestamps
  const [loading, setLoading] = useState(true); // Estado de carga

  useEffect(() => {
    const docRef = doc(db, "data", "signal"); // Referencia al documento de Firestore

    const unsubscribe = onSnapshot(docRef, (snapshot) => {
      if (snapshot.exists()) {
        const data = snapshot.data();
        const newValues = [];

        // Extraer valores de ecg1...ecg100
        for (let i = 1; i <= 100; i++) {
          const key = `ecg${i}`;
          if (data[key]) {
            newValues.push(data[key]);
          }
        }

        if (newValues.length > 0) {
          setEcg1Data((prevData) => {
            const updatedData = [...prevData, ...newValues];

            // Mantener solo los últimos 2000 valores
            if (updatedData.length > MAX_DATA_POINTS) {
              updatedData.splice(0, updatedData.length - MAX_DATA_POINTS);
            }

            return updatedData;
          });

          setTimestamps((prevTimestamps) => {
            const newTimestamps = Array.from(
              { length: newValues.length },
              (_, i) => prevTimestamps.length + i
            );

            const updatedTimestamps = [...prevTimestamps, ...newTimestamps];

            // Mantener solo los últimos 2000 timestamps
            if (updatedTimestamps.length > MAX_DATA_POINTS) {
              updatedTimestamps.splice(0, updatedTimestamps.length - MAX_DATA_POINTS);
            }

            return updatedTimestamps;
          });
        }
        setLoading(false);
      } else {
        console.log("No se encontró el documento en Firestore");
        setLoading(false);
      }
    });

    return () => unsubscribe();
  }, []);

  const chartConfig = {
    backgroundColor: "#FFFFFF", // Fondo blanco
    backgroundGradientFrom: "#FFFFFF",
    backgroundGradientTo: "#FFFFFF",
    decimalPlaces: 2,
    color: (opacity = 1) => `rgba(139, 0, 0, ${opacity})`, // Rojo oscuro para la línea
    labelColor: (opacity = 1) => `rgba(0, 0, 0, ${opacity})`, // Etiquetas negras
    propsForDots: {
      r: "0", // Ocultar puntos en las líneas
    },
    propsForBackgroundLines: {
      stroke: "#D3D3D3", // Líneas del grid
      strokeWidth: 1,
    },
    style: { borderRadius: 16 },
  };

  return (
    <ScrollView style={styles.container}>
      <Text style={styles.title}>Gráfico de ECG</Text>

      {loading ? (
        <Text style={styles.loading}>Cargando datos...</Text>
      ) : (
        <>
          {ecg1Data.length > 0 && (
            <View>
              <Text style={styles.subtitle}>ECG1</Text>
              <LineChart
                data={{
                  labels: timestamps
                    .slice(-WINDOW_SIZE)
                    .map((time) => time.toString()),
                  datasets: [
                    {
                      data: ecg1Data.slice(-WINDOW_SIZE).map(Number),
                      strokeWidth: 2, // Ancho de la línea
                    },
                  ],
                }}
                width={Dimensions.get("window").width - 40}
                height={220}
                chartConfig={{
                  ...chartConfig,
                  fillShadowGradientOpacity: 0, // Eliminar la región sombreada debajo
                }}
                style={styles.chart}
              />
            </View>
          )}
        </>
      )}
    </ScrollView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: "#FFFFFF",
    paddingTop: 20,
  },
  title: {
    fontSize: 24,
    fontWeight: "bold",
    textAlign: "center",
    marginBottom: 20,
    color: "#FF0000",
  },
  subtitle: {
    fontSize: 20,
    fontWeight: "bold",
    marginBottom: 10,
    textAlign: "center",
    color: "#8B0000", // Rojo oscuro
  },
  loading: {
    fontSize: 18,
    textAlign: "center",
    color: "#4C51BF",
  },
  chart: {
    marginVertical: 20,
    borderRadius: 16,
  },
});

export default Tutorial;









