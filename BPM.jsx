import React, { useState, useEffect } from 'react';
import { View, Text, TouchableOpacity, StyleSheet, Alert, Image } from 'react-native';
import { useRouter } from 'expo-router';
import { getDatabase, ref, onValue, set } from 'firebase/database'; // Importar funciones de Firebase
import * as Location from 'expo-location'; // Importar API de ubicación

const BPMScreen = () => {
  const router = useRouter();
  const [bpmData, setBpmData] = useState(null); // Estado para BPM
  const [loading, setLoading] = useState(true); // Estado de carga
  const [address, setAddress] = useState(''); // Estado para la dirección
  const [locationReady, setLocationReady] = useState(false); // Estado para verificar si la ubicación está lista

  useEffect(() => {
    // Solicitar permisos de ubicación
    const requestLocationPermission = async () => {
      let { status } = await Location.requestForegroundPermissionsAsync();
      if (status !== 'granted') {
        Alert.alert(
          'Permiso denegado',
          'No podemos acceder a tu ubicación. Por favor habilítala en la configuración.'
        );
        return;
      }

      // Obtener ubicación actual
      let currentLocation = await Location.getCurrentPositionAsync({});
      const { latitude, longitude } = currentLocation.coords;

      // Obtener dirección a partir de las coordenadas
      let reverseGeocode = await Location.reverseGeocodeAsync({
        latitude,
        longitude,
      });

      // Guardar la primera dirección encontrada en el estado
      if (reverseGeocode.length > 0) {
        const location = reverseGeocode[0];
        setAddress(
          `${location.name}, ${location.street}, ${location.city}, ${location.region}, ${location.country}`
        );
        setLocationReady(true); // La ubicación está lista
      } else {
        setAddress('Dirección no disponible');
        setLocationReady(true); // La ubicación está lista incluso si no se obtuvo dirección
      }
    };

    requestLocationPermission();

    const db = getDatabase(); // Inicializar la base de datos
    const bpmRef = ref(db, 'test/BPM'); // Ruta para BPM en Firebase
    const alarmaRef = ref(db, 'test/Alarma'); // Ruta para Alarma en Firebase

    // Listener para BPM
    const unsubscribe = onValue(
      bpmRef,
      (snapshot) => {
        const value = snapshot.val();
        if (value !== null) {
          setBpmData(value); // Actualizar estado con el BPM

          // Mostrar alerta y actualizar Firebase si el BPM supera el límite
          if (value > 90) {
            // Enviar alerta al usuario
            Alert.alert(
              '¡ALERTA!',
              `DETECCIÓN DE ARRITMIA, SE ESTÁ AVISANDO A SUS FAMILIARES.\n\nUbicación: ${address}`,
              [{ text: 'OK' }]
            );

            // Actualizar el valor de Alarma en Firebase
            set(alarmaRef, 1).then(() => {
              console.log('Alarma activada en Firebase');
            }).catch((error) => {
              console.error('Error al actualizar la alarma:', error);
            });
          }
        } else {
          setBpmData('NO DISPONIBLE'); // Si no hay datos
        }
        setLoading(false); // Desactivar estado de carga
      },
      (error) => {
        console.error('Error al leer BPM:', error.message);
        Alert.alert('Error', 'No se pudo obtener el BPM.');
        setLoading(false);
      }
    );

    return () => unsubscribe(); // Limpiar el listener cuando se desmonte el componente
  }, [address, locationReady]);

  return (
    <View style={styles.container}>
      <Text style={styles.title}>FRECUENCIA CARDIACA</Text>

      {loading ? (
        <Text style={styles.loading}>CARGANDO...</Text>
      ) : (
        <View style={styles.bpmContainer}>
          <Text style={styles.bpmLabel}>BPM:</Text>
          <Text style={styles.bpmValue}>{bpmData}</Text>
        </View>
      )}

      <TouchableOpacity style={styles.button} onPress={() => router.back()}>
        <Text style={styles.buttonText}>VOLVER</Text>
      </TouchableOpacity>

      {/* Imagen Doctor */}
      <Image source={require('./doctor.png')} style={styles.logo} />
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F3F4F6',
    padding: 20,
  },
  title: {
    fontSize: 28,
    fontWeight: 'bold',
    color: '#FF3D71', // Rojo vibrante
    textAlign: 'center',
    marginBottom: 30,
    textTransform: 'uppercase', // Texto en mayúsculas
  },
  loading: {
    fontSize: 18,
    color: '#9CA3AF', // Gris claro
    textAlign: 'center',
    marginBottom: 20,
  },
  bpmContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    backgroundColor: '#FFFFFF',
    paddingVertical: 20,
    paddingHorizontal: 40,
    borderRadius: 15,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 4 },
    shadowOpacity: 0.2,
    shadowRadius: 4,
    elevation: 5, // Sombra para Android
    marginBottom: 30,
  },
  bpmLabel: {
    fontSize: 24,
    fontWeight: 'bold',
    color: '#4B5563', // Gris oscuro
    marginRight: 10,
    textTransform: 'uppercase', // Texto en mayúsculas
  },
  bpmValue: {
    fontSize: 36,
    fontWeight: 'bold',
    color: '#10B981', // Verde vibrante
  },
  button: {
    backgroundColor: '#FF3D71', // Rojo vibrante
    paddingVertical: 15,
    paddingHorizontal: 30,
    borderRadius: 25,
    alignItems: 'center',
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.2,
    shadowRadius: 4,
    elevation: 5, // Sombra en Android
    marginBottom: 20,
  },
  buttonText: {
    color: '#FFFFFF', // Texto blanco
    fontSize: 18,
    fontWeight: 'bold',
    textTransform: 'uppercase', // Texto en mayúsculas
  },
  logo: {
    width: 150, // Ajusta el ancho de la imagen
    height: 150, // Ajusta el alto de la imagen
    resizeMode: 'contain', // Ajusta para que la imagen mantenga sus proporciones
    marginTop: 20, // Espacio superior respecto al botón
  },
});

export default BPMScreen;











