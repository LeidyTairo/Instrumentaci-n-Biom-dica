import React from 'react';
import { View, Text, TouchableOpacity, StyleSheet, ScrollView, Image } from 'react-native';
import { useRouter } from 'expo-router';

const HomeScreen = () => {
  const router = useRouter();

  const handleStart = () => {
    router.push('/patient-form'); // Navegar a la pantalla del formulario
  };

  return (
    <ScrollView contentContainerStyle={styles.container}>
      {/* Logo */}
      <Image source={require('./Logo.png')} style={styles.logo} />

      {/* Títulos */}
      <Text style={styles.title}>SmartBeat</Text>
      <Text style={styles.subtitle}>¡BIENVENIDO!</Text>

      {/* Botones */}
      <TouchableOpacity style={styles.button} onPress={handleStart}>
        <Text style={styles.buttonText}>Iniciar sesión</Text>
      </TouchableOpacity>

      <TouchableOpacity style={styles.secondaryButton}>
        <Text style={styles.secondaryButtonText}>Continuar sesión</Text>
      </TouchableOpacity>
    </ScrollView>
  );
};

const styles = StyleSheet.create({
  container: {
    flexGrow: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 20,
    backgroundColor: '#FFE4E6', // Fondo rosado claro
  },
  logo: {
    width: 150,
    height: 150,
    marginBottom: 20,
  },
  title: {
    fontSize: 40,
    fontWeight: 'bold',
    color: '#B91C1C', // Rojo oscuro
    textAlign: 'center',
    marginBottom: 10,
    textTransform: 'uppercase',
  },
  subtitle: {
    fontSize: 20,
    color: '#F43F5E', // Rosa fuerte
    textAlign: 'center',
    marginBottom: 40,
  },
  button: {
    backgroundColor: '#BE185D', // Morado-rosado vibrante
    paddingVertical: 15,
    paddingHorizontal: 50,
    borderRadius: 25,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.3,
    shadowRadius: 3,
    elevation: 5, // Sombra en Android
    marginBottom: 20,
  },
  buttonText: {
    color: '#FFFFFF', // Texto blanco
    fontSize: 18,
    fontWeight: 'bold',
    textTransform: 'uppercase',
  },
  secondaryButton: {
    backgroundColor: '#BE185D', // Rosa claro
    paddingVertical: 15,
    paddingHorizontal: 50,
    borderRadius: 25,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.3,
    shadowRadius: 3,
    elevation: 5, // Sombra en Android
    marginBottom: 20,
  },
  secondaryButtonText: {
    color: '#FFFFFF', // Rojo oscuro
    fontSize: 18,
    fontWeight: 'bold',
    textTransform: 'uppercase',
  },
});

export default HomeScreen;




