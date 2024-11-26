import React, { useState } from 'react';
import { View, Text, TouchableOpacity, Modal, StyleSheet, Alert, ScrollView, KeyboardAvoidingView, Platform } from 'react-native';
import { useRouter } from 'expo-router';

const ExamConfig = () => {
  const [category, setCategory] = useState('');
  const [showCategoryModal, setShowCategoryModal] = useState(false);
  const router = useRouter(); // Hook para manejar la navegación

  const handleSave = async () => {
    if (!category) {
      Alert.alert('Error', 'Debe seleccionar una categoría.');
      return;
    }

    try {
      Alert.alert('Éxito', 'Categoría guardada correctamente.');
      router.push('/tutorial');
    } catch (error) {
      console.error("Error: ", error);
      Alert.alert('Error', 'No se pudo guardar la categoría.');
    }
  };

  const handleBPM = () => {
    router.push('/BPM'); // Navegar a la pantalla BPMScreen
  };

  return (
    <KeyboardAvoidingView
      behavior={Platform.OS === 'ios' ? 'padding' : 'height'}
      style={{ flex: 1 }}
      keyboardVerticalOffset={100}
    >
      <ScrollView contentContainerStyle={styles.container}>
        <Text style={styles.title}>Configuración del Examen</Text>

        {/* Etiqueta para Categoría */}
        <Text style={styles.label}>Seleccione la Categoría</Text>
        <TouchableOpacity
          style={styles.pickerToggle}
          onPress={() => setShowCategoryModal(true)}
        >
          <Text style={styles.pickerText}>{category || 'Seleccione categoría'}</Text>
        </TouchableOpacity>

        {/* Modal para seleccionar Categoría */}
        <Modal
          transparent={true}
          visible={showCategoryModal}
          animationType="slide"
          onRequestClose={() => setShowCategoryModal(false)}
        >
          <View style={styles.modalContainer}>
            <View style={styles.modalContent}>
              <Text style={styles.modalTitle}>Seleccionar Categoría</Text>
              <TouchableOpacity onPress={() => { setCategory('Paciente'); setShowCategoryModal(false); }}>
                <Text style={styles.modalOption}>Paciente</Text>
              </TouchableOpacity>
              <TouchableOpacity onPress={() => { setCategory('Familiar'); setShowCategoryModal(false); }}>
                <Text style={styles.modalOption}>Familiar</Text>
              </TouchableOpacity>
            </View>
          </View>
        </Modal>

        {/* Botón Guardar y Continuar */}
        <TouchableOpacity style={styles.button} onPress={handleSave}>
          <Text style={styles.buttonText}>Guardar y Continuar</Text>
        </TouchableOpacity>

        {/* Botón BPM */}
        <TouchableOpacity style={styles.button} onPress={handleBPM}>
          <Text style={styles.buttonText}>BPM</Text>
        </TouchableOpacity>
      </ScrollView>
    </KeyboardAvoidingView>
  );
};

const styles = StyleSheet.create({
  container: {
    flexGrow: 1,
    padding: 20,
    backgroundColor: '#FFE4E6', // Fondo rosado claro
    justifyContent: 'center',
  },
  title: {
    fontSize: 28,
    fontWeight: 'bold',
    color: '#B91C1C', // Rojo oscuro
    textAlign: 'center',
    marginBottom: 30,
  },
  label: {
    fontSize: 16,
    color: '#F43F5E', // Rosa fuerte
    marginBottom: 8,
  },
  pickerToggle: {
    paddingVertical: 10,
    paddingHorizontal: 15,
    borderWidth: 1,
    borderColor: '#FECACA', // Rosa claro
    borderRadius: 10,
    marginBottom: 20,
    backgroundColor: '#FFFFFF',
  },
  pickerText: {
    fontSize: 16,
    color: '#B91C1C', // Rojo oscuro
  },
  button: {
    backgroundColor: '#BE185D', // Morado-rosado vibrante
    paddingVertical: 15,
    borderRadius: 25,
    alignItems: 'center',
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.3,
    shadowRadius: 3,
    elevation: 5, // Sombra en Android
    marginTop: 20, // Mayor espacio entre el último campo y el botón
  },
  buttonText: {
    color: '#FFFFFF',
    fontSize: 18,
    fontWeight: 'bold',
    textTransform: 'uppercase',
  },
  modalContainer: {
    flex: 1,
    justifyContent: 'center',
    backgroundColor: 'rgba(0, 0, 0, 0.5)',
  },
  modalContent: {
    backgroundColor: '#FFFFFF',
    margin: 20,
    padding: 20,
    borderRadius: 10,
  },
  modalTitle: {
    fontSize: 18,
    fontWeight: 'bold',
    marginBottom: 10,
    color: '#B91C1C', // Rojo oscuro
  },
  modalOption: {
    fontSize: 16,
    paddingVertical: 10,
    color: '#F43F5E', // Rosa fuerte
  },
});

export default ExamConfig;




