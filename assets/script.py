import os

def delete_and_rename_images(directory, delete_list):
    # Obter todos os arquivos .jpeg na pasta
    all_images = [f for f in os.listdir(directory) if f.endswith('.jpeg')]
    
    # Filtrar os números das imagens
    image_numbers = sorted([int(f.split('.')[0]) for f in all_images])
    
    # Deletar as imagens especificadas na lista delete_list
    for img_number in delete_list:
        if img_number in image_numbers:
            os.remove(os.path.join(directory, f"{img_number}.jpeg"))
            image_numbers.remove(img_number)  # Remover da lista após deletar
            print(f"Imagem {img_number}.jpeg excluída.")
    
    # Renomear as imagens restantes para manter a ordem sequencial
    for new_number, old_number in enumerate(image_numbers):
        old_image = os.path.join(directory, f"{old_number}.jpeg")
        new_image = os.path.join(directory, f"{new_number}.jpeg")
        
        os.rename(old_image, new_image)
        print(f"Imagem {old_number}.jpeg renomeada para {new_number}.jpeg.")

# Exemplo de uso
directory = 'arts'  # Substitua pelo caminho real da pasta
delete_list = [150, 149, 144, 145, 146, 147, 152, 153, 154, 159, 158, 157, 135, 134, 132, 129, 130,
136, 137, 138, 143, 141, 140, 119, 118, 116, 112, 113]  # Exemplo de lista de imagens a excluir

print(len(delete_list))
# exit(1)
delete_and_rename_images(directory, delete_list)
